#include <hpp/MyCommand.hpp>
#include <raylib.h>

int main() {
	bool play = true;

	Points::Circle* cir = new Points::Circle();
	vector<Points::Solving*> points;

	LikeConsole::Console* console = new LikeConsole::Console({
		{"clear", new LikeConsole::Commands::Clear()},
		{"set_font_size", new LikeConsole::Commands::SetFontSize()},
		{"exit", new LikeConsole::Commands::Exit(play)},
		{"help", new LikeConsole::Commands::Help()},
		{"color", new MyCommands::Color(cir, points)},
		{"add", new MyCommands::AddPoint(points)},
		{"list", new MyCommands::List(points)},
		{"delete", new MyCommands::Delete(points)},
		{"hide", new MyCommands::Hide(points)},
		{"show", new MyCommands::Show(points)}
		}, 24);

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(500, 500, "Trigonometric Circle");
	SetWindowMinSize(100, 100);

	while (!WindowShouldClose() && play) {
		console->logic(GetScreenWidth() / 5, GetScreenHeight() / 5, 3 * GetScreenWidth() / 5, 3 * GetScreenHeight() / 5);

		BeginDrawing();
		ClearBackground(WHITE);

		float r = min(GetScreenWidth(), GetScreenHeight()) * 0.45;

		console->draw(GetScreenWidth() / 2 - r / sqrt(2) + 12, GetScreenHeight() / 2 - r / sqrt(2) + 12, 2 * r / sqrt(2) - 24, 2 * r / sqrt(2) - 24);

		cir->draw();
		for (int i = 0; i < points.size(); ++i)
			points[i]->draw();

		EndDrawing();
	}

	CloseWindow();

	delete console;
	delete cir;
	for (int i = 0; i < points.size(); ++i)
		delete points[i];
	return 0;
}