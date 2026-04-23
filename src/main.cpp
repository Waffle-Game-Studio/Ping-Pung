#include <cmath>
#include <components/button.hpp>
#include <components/text.hpp>
#include <raylib.h>
#include <screens/screen_manager.hpp>
#include <steam/steam_api.h>
#include <string>
#include <vector>

static void StyleMenuButton(ButtonComponent &btn, const Color &normal,
                            const Color &hover, const Color &pressed) {
  btn.background_color = normal;
  btn.hover_enter_handlers.push_back(
      [=](ButtonComponent &b) { b.background_color = hover; });
  btn.hover_exit_handlers.push_back(
      [=](ButtonComponent &b) { b.background_color = normal; });
  btn.pressed_handlers.push_back(
      [=](ButtonComponent &b) { b.background_color = pressed; });
  btn.released_handlers.push_back([=](ButtonComponent &b) {
    b.background_color = b.hovered ? hover : normal;
  });
}

int main() {
  if (SteamAPI_RestartAppIfNecessary(480)) {
    return 1;
  }

  if (!SteamAPI_Init()) {
    return 1;
  }

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Ping Pung");
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);

  // Font font = LoadFont("assets/fonts/Pixel.ttf");
  Font font = GetFontDefault();

  const Color NO_BG = Color{0, 0, 0, 0};
  const Color BG_COLOR = Color{18, 22, 32, 255};

  const Color BTN_NORMAL = Color{60, 70, 95, 255};
  const Color BTN_HOVER = Color{95, 125, 190, 255};
  const Color BTN_PRESSED = Color{40, 55, 120, 255};

  const Color ACCENT_NORMAL = Color{50, 130, 90, 255};
  const Color ACCENT_HOVER = Color{80, 190, 130, 255};
  const Color ACCENT_PRESSED = Color{30, 95, 65, 255};

  const Color QUIT_NORMAL = Color{120, 45, 55, 255};
  const Color QUIT_HOVER = Color{200, 70, 80, 255};
  const Color QUIT_PRESSED = Color{140, 35, 45, 255};

  ScreenManager sm;
  bool shouldQuit = false;

  // ---------------- Title & subtitle ----------------
  TextComponent titleText("PING PUNG", UDim2{0.5f, 0.18f, 0.0f, 0.0f}, font,
                          UDim{0.12f, 0.0f}, 4.0f, WHITE, NO_BG);

  TextComponent subtitleText("a tiny paddle game",
                             UDim2{0.5f, 0.30f, 0.0f, 0.0f}, font,
                             UDim{0.03f, 0.0f}, 2.0f, LIGHTGRAY, NO_BG);

  // Footer / hint line.
  TextComponent hintText("Use the mouse to navigate",
                         UDim2{0.5f, 0.95f, 0.0f, 0.0f}, font,
                         UDim{0.018f, 0.0f}, 1.0f, GRAY, NO_BG);

  // Steam login status (top-right).
  TextComponent steamText("", UDim2{1.0f, 0.0f, -12.0f, 12.0f}, font,
                          UDim{0.0f, 16.0f}, 1.0f, LIME, Color{0, 0, 0, 160});
  steamText.pivot = Vector2{1.0f, 0.0f};
  steamText.background_padding = {0.0f, 0.0f, 12.0f, 6.0f};

  // ---------------- Main menu buttons ----------------
  // Font height scales with screen (3.5%), padding is 32px horizontal / 14px
  // vertical around the text.
  const UDim btnFont{0.035f, 0.0f};
  const UDim2 btnPad{0.0f, 0.0f, 32.0f, 14.0f};

  ButtonComponent playBtn("Play", UDim2{0.5f, 0.48f, 0.0f, 0.0f}, btnFont,
                          btnPad, font, 2.0f, WHITE, ACCENT_NORMAL);
  StyleMenuButton(playBtn, ACCENT_NORMAL, ACCENT_HOVER, ACCENT_PRESSED);
  playBtn.click_handlers.push_back(
      [&sm](ButtonComponent &) { sm.SetActive("play"); });

  ButtonComponent settingsBtn("Settings", UDim2{0.5f, 0.60f, 0.0f, 0.0f},
                              btnFont, btnPad, font, 2.0f, WHITE, BTN_NORMAL);
  StyleMenuButton(settingsBtn, BTN_NORMAL, BTN_HOVER, BTN_PRESSED);
  settingsBtn.click_handlers.push_back(
      [&sm](ButtonComponent &) { sm.SetActive("settings"); });

  ButtonComponent creditsBtn("Credits", UDim2{0.5f, 0.72f, 0.0f, 0.0f}, btnFont,
                             btnPad, font, 2.0f, WHITE, BTN_NORMAL);
  StyleMenuButton(creditsBtn, BTN_NORMAL, BTN_HOVER, BTN_PRESSED);
  creditsBtn.click_handlers.push_back(
      [&sm](ButtonComponent &) { sm.SetActive("credits"); });

  ButtonComponent quitBtn("Quit", UDim2{0.5f, 0.84f, 0.0f, 0.0f}, btnFont,
                          btnPad, font, 2.0f, WHITE, QUIT_NORMAL);
  StyleMenuButton(quitBtn, QUIT_NORMAL, QUIT_HOVER, QUIT_PRESSED);
  quitBtn.click_handlers.push_back(
      [&shouldQuit](ButtonComponent &) { shouldQuit = true; });

  // ---------------- Sub-screen header + back button ----------------
  TextComponent subHeader("", UDim2{0.5f, 0.2f, 0.0f, 0.0f}, font,
                          UDim{0.08f, 0.0f}, 2.0f, WHITE, NO_BG);

  TextComponent subBody("", UDim2{0.5f, 0.5f, 0.0f, 0.0f}, font,
                        UDim{0.03f, 0.0f}, 1.5f, LIGHTGRAY, NO_BG);

  ButtonComponent backBtn("< Back", UDim2{0.5f, 0.85f, 0.0f, 0.0f},
                          UDim{0.028f, 0.0f}, UDim2{0.0f, 0.0f, 24.0f, 10.0f},
                          font, 1.5f, WHITE, BTN_NORMAL);
  StyleMenuButton(backBtn, BTN_NORMAL, BTN_HOVER, BTN_PRESSED);
  backBtn.click_handlers.push_back(
      [&sm](ButtonComponent &) { sm.SetActive("main"); });

  // ---------------- Ping-Pong gameplay state ----------------
  // All positions/sizes are in normalized screen space (0..1) so the game
  // adapts to window resizing.
  const float PADDLE_W = 0.014f; // fraction of screen width
  const float PADDLE_H = 0.18f;  // fraction of screen height
  const float BALL_R = 0.014f;   // fraction of screen height
  const float PLAYER_X = 0.05f;
  const float AI_X = 0.95f;
  const float PLAYER_SPEED = 1.1f; // screens per second (vertical)
  const float AI_SPEED = 0.75f;
  const float BALL_SPEED_START = 0.65f; // screens per second
  const float BALL_SPEED_MAX = 1.3f;
  const float BALL_SPEED_GAIN = 1.04f; // per paddle hit

  float playerY = 0.5f;
  float aiY = 0.5f;
  Vector2 ballPos = {0.5f, 0.5f};
  Vector2 ballVel = {BALL_SPEED_START, BALL_SPEED_START * 0.5f};
  float ballSpeed = BALL_SPEED_START;
  int playerScore = 0;
  int aiScore = 0;
  int serveDir = 1; // +1 serves to AI, -1 to player

  auto resetBall = [&](int dir) {
    ballPos = {0.5f, 0.5f};
    ballSpeed = BALL_SPEED_START;
    float angle = (GetRandomValue(-30, 30))*DEG2RAD;
    ballVel = {dir * ballSpeed * cosf(angle), ballSpeed * sinf(angle)};
  };

  auto resetMatch = [&]() {
    playerScore = 0;
    aiScore = 0;
    playerY = 0.5f;
    aiY = 0.5f;
    serveDir = (GetRandomValue(0, 1) == 0) ? -1 : 1;
    resetBall(serveDir);
  };

  // In-game HUD.
  TextComponent scoreText("0   0", UDim2{0.5f, 0.08f, 0.0f, 0.0f}, font,
                          UDim{0.09f, 0.0f}, 6.0f, WHITE, NO_BG);
  TextComponent escHintText("ESC: back to menu", UDim2{0.5f, 0.95f, 0.0f, 0.0f},
                            font, UDim{0.018f, 0.0f}, 1.0f, GRAY, NO_BG);

  // ---------------- Build screens ----------------
  Screen mainScreen;
  mainScreen.Add(&titleText);
  mainScreen.Add(&subtitleText);
  mainScreen.Add(&playBtn);
  mainScreen.Add(&settingsBtn);
  mainScreen.Add(&creditsBtn);
  mainScreen.Add(&quitBtn);
  mainScreen.Add(&hintText);

  Screen settingsScreen;
  settingsScreen.Add(&subHeader);
  settingsScreen.Add(&subBody);
  settingsScreen.Add(&backBtn);
  settingsScreen.Add(&hintText);
  settingsScreen.on_enter = [&]() {
    subHeader.text = "Settings";
    subBody.text = "Nothing to configure... yet.";
  };

  Screen creditsScreen;
  creditsScreen.Add(&subHeader);
  creditsScreen.Add(&subBody);
  creditsScreen.Add(&backBtn);
  creditsScreen.Add(&hintText);
  creditsScreen.on_enter = [&]() {
    subHeader.text = "Credits";
    subBody.text = "Ping Pung\nby Waffle Game Studio";
  };

  Screen playScreen;
  playScreen.Add(&scoreText);
  playScreen.Add(&escHintText);
  playScreen.on_enter = [&]() { resetMatch(); };
  playScreen.on_update = [&]() {
    // ESC returns to menu.
    if (IsKeyPressed(KEY_ESCAPE)) {
      sm.SetActive("main");
      return;
    }

    float dt = GetFrameTime();

    // --- Player input ---
    float dir = 0.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      dir -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      dir += 1.0f;
    playerY += dir * PLAYER_SPEED * dt;

    float halfPaddle = PADDLE_H * 0.5f;
    if (playerY < halfPaddle)
      playerY = halfPaddle;
    if (playerY > 1.0f - halfPaddle)
      playerY = 1.0f - halfPaddle;

    // --- AI: chase ball y, capped by AI_SPEED ---
    float aiDelta = ballPos.y - aiY;
    float aiStep = AI_SPEED * dt;
    if (aiDelta > aiStep)
      aiDelta = aiStep;
    if (aiDelta < -aiStep)
      aiDelta = -aiStep;
    aiY += aiDelta;
    if (aiY < halfPaddle)
      aiY = halfPaddle;
    if (aiY > 1.0f - halfPaddle)
      aiY = 1.0f - halfPaddle;

    // --- Ball movement ---
    ballPos.x += ballVel.x * dt;
    ballPos.y += ballVel.y * dt;

    // Top/bottom bounce.
    if (ballPos.y < BALL_R) {
      ballPos.y = BALL_R;
      ballVel.y = -ballVel.y;
    } else if (ballPos.y > 1.0f - BALL_R) {
      ballPos.y = 1.0f - BALL_R;
      ballVel.y = -ballVel.y;
    }

    // Paddle collision (reflect + angle shift based on hit offset).
    auto collidePaddle = [&](float paddleX, float paddleY, int sideSign) {
      float halfW = PADDLE_W * 0.5f;
      float paddleLeft = paddleX - halfW;
      float paddleRight = paddleX + halfW;
      float paddleTop = paddleY - halfPaddle;
      float paddleBottom = paddleY + halfPaddle;

      bool overlapsX =
          ballPos.x + BALL_R >= paddleLeft && ballPos.x - BALL_R <= paddleRight;
      bool overlapsY = ballPos.y >= paddleTop && ballPos.y <= paddleBottom;
      bool movingToward = (sideSign < 0 && ballVel.x < 0.0f) ||
                          (sideSign > 0 && ballVel.x > 0.0f);

      if (overlapsX && overlapsY && movingToward) {
        ballSpeed = fminf(ballSpeed * BALL_SPEED_GAIN, BALL_SPEED_MAX);
        float offset = (ballPos.y - paddleY) / halfPaddle;
        if (offset < -1.0f)
          offset = -1.0f;
        if (offset > 1.0f)
          offset = 1.0f;
        float maxBounce = 55.0f * DEG2RAD;
        float angle = offset * maxBounce;
        float xdir = (sideSign < 0) ? 1.0f : -1.0f;
        ballVel.x = xdir * ballSpeed * cosf(angle);
        ballVel.y = ballSpeed * sinf(angle);
        ballPos.x =
            (sideSign < 0) ? (paddleRight + BALL_R) : (paddleLeft - BALL_R);
      }
    };

    collidePaddle(PLAYER_X, playerY, -1);
    collidePaddle(AI_X, aiY, +1);

    // Scoring.
    if (ballPos.x < -BALL_R) {
      aiScore++;
      resetBall(+1);
    } else if (ballPos.x > 1.0f + BALL_R) {
      playerScore++;
      resetBall(-1);
    }

    scoreText.text =
        std::to_string(playerScore) + "   " + std::to_string(aiScore);
  };
  // Draw everything (playfield + HUD) inside on_draw so the ball/paddles can
  // sit under the score text.
  playScreen.components.clear();
  playScreen.on_draw = [&]() {
    float sw = static_cast<float>(GetScreenWidth());
    float sh = static_cast<float>(GetScreenHeight());

    // Dashed center line.
    float dashH = sh * 0.025f;
    float dashGap = sh * 0.015f;
    float dashW = sw * 0.004f;
    for (float y = 0.0f; y < sh; y += dashH + dashGap) {
      DrawRectangle(static_cast<int>(sw * 0.5f - dashW * 0.5f),
                    static_cast<int>(y), static_cast<int>(dashW),
                    static_cast<int>(dashH), Color{60, 70, 90, 255});
    }

    // Player paddle (green).
    float pw = sw * PADDLE_W;
    float ph = sh * PADDLE_H;
    DrawRectangleRec(
        Rectangle{sw * PLAYER_X - pw * 0.5f, sh * playerY - ph * 0.5f, pw, ph},
        Color{120, 210, 160, 255});

    // AI paddle (red).
    DrawRectangleRec(
        Rectangle{sw * AI_X - pw * 0.5f, sh * aiY - ph * 0.5f, pw, ph},
        Color{210, 120, 130, 255});

    // Ball.
    DrawCircleV(Vector2{sw * ballPos.x, sh * ballPos.y}, sh * BALL_R, WHITE);

    // HUD on top.
    scoreText.Update();
    escHintText.Update();
    scoreText.Draw();
    escHintText.Draw();
  };

  sm.Register("main", &mainScreen);
  sm.Register("play", &playScreen);
  sm.Register("settings", &settingsScreen);
  sm.Register("credits", &creditsScreen);
  sm.SetActive("main");

  while (!WindowShouldClose() && !shouldQuit) {
    SteamAPI_RunCallbacks();

    // Steam status line (always visible overlay).
    std::string steamInfo;
    if (SteamUser() != nullptr && SteamUser()->BLoggedOn() &&
        SteamFriends() != nullptr) {
      steamInfo =
          std::string("Signed in as ") + SteamFriends()->GetPersonaName();
    } else {
      steamInfo = "Steam: offline";
    }
    steamText.text = steamInfo;
    steamText.Update();

    sm.Update();

    BeginDrawing();
    ClearBackground(BG_COLOR);
    sm.Draw();
    steamText.Draw();
    EndDrawing();
  }

  SteamAPI_Shutdown();
  CloseWindow();
  return 0;
}
