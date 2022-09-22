#include <wlr/backend/headless.h>
#include <wlr/backend/multi.h>
#include <wlr/backend/wayland.h>
#include "sway/commands.h"
#include "sway/output.h"
#include "sway/server.h"
#include "log.h"
#include "stringop.h"

static void destroy_output(struct wlr_backend *backend, void *data) {
	bool *done = data;
	if (*done) {
		return;
	}

	sway_log(SWAY_DEBUG, "Destroying last headless output %s", last_headless_name());

  struct sway_output *output = output_by_name_or_id(last_headless_name());
  if (!output) {
    *done = true;
    return;
  }

  wlr_output_destroy(output->wlr_output);
  *done = true;
}

/**
 * This command is intended for developer use only.
 */
struct cmd_results *cmd_destroy_output(int argc, char **argv) {
	sway_assert(wlr_backend_is_multi(server.backend),
			"Expected a multi backend");

	bool done = false;
  // char *output_name = join_args(argv, argc);
	wlr_multi_for_each_backend(server.backend, destroy_output, &done);

	if (!done) {
		return cmd_results_new(CMD_INVALID,
			"Can only destroy outputs for Wayland, X11 or headless backends");
	}

	return cmd_results_new(CMD_SUCCESS, NULL);
}
