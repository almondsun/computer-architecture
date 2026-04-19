#ifndef SCROLLING_SIGN_APP_H
#define SCROLLING_SIGN_APP_H

/**
 * @brief Run the scrolling-sign firmware forever.
 *
 * `message` is borrowed for the lifetime of the application loop.
 *
 * @param message Zero-terminated text to render on the display.
 */
void scrolling_sign_app_run(const char *message);

#endif // SCROLLING_SIGN_APP_H
