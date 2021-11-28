

#include <cassert>
#include <vector>
#include <queue>
#include <cmath>

#include "Vec2D.h"
#include <SFML/Graphics.hpp>

void shapeSetPosition(sf::Shape& shape, Vec2D position)
{
	shape.setPosition(position.m_x, position.m_y);
}

struct Pixel
{
	Vec2D position;
	int team_id = 0;
	int visited = -1;
};


void clear_visited(std::vector<Pixel>& map)
{
	for (auto& pixel : map) {
		pixel.visited = -1;
	}
}

struct Unit
{
	Vec2D position;
	Vec2D direction;

	int team_id = 1;
	sf::CircleShape shape;
	sf::Color color;

	Unit(Vec2D p, int t, sf::Color c)
	: position(p)
	, team_id(t)
	, shape(20.f)
	, color(c)
	, direction(0.0f, 0.0f)
	{
		shape.setFillColor(color);
	}
};

const int size_map = 40;
const float size_pixel = 25.0f;

void createMap(std::vector<Pixel>& map)
{
	for (int x = 0; x < size_map; ++x)
	{
		for (int y = 0; y < size_map; ++y)
		{
			Pixel pixel;
			pixel.position = Vec2D(size_pixel *x, size_pixel *y);
			pixel.team_id = 0;
			map.emplace_back(std::move(pixel));
		}
	}
}

void checkLoop(std::vector<Pixel>& map, int x, int y, int team_id);

void fillColorRec(std::vector<Pixel>& map, int x, int y, int team_id)
{
	int index = x * size_map + y;
	// checkLoop(map, x, y, team_id);
	map[index].team_id = team_id;


	// Right
	if (x < size_map - 1) {
		int index = (x + 1) * size_map + y;
		if (map[index].team_id != team_id) {
			fillColorRec(map, x + 1, y, team_id);
		}
	}
	// left
	if (x > 1) {
		int index = (x - 1) * size_map + y;
		if (map[index].team_id != team_id) {
			fillColorRec(map, x - 1, y, team_id);
		}
	}
	// Down
	if (y < size_map - 1) {
		int index = x * size_map + (y + 1);
		if (map[index].team_id != team_id) {
			fillColorRec(map, x, y + 1, team_id);
		}
	}
	if (y > 1) {
		int index = x * size_map + (y - 1);
		if (map[index].team_id != team_id) {
			fillColorRec(map, x, y - 1, team_id);
		}
	}
}

void fillColor(std::vector<Pixel>& map, std::vector<int>&& path, int team_id)
{
	if (path.size() < 8)
		return;
	
	int minX = size_map;
	int maxX = -1;
	int minY = size_map;
	int maxY = -1;
	int xIn = path[0] / size_map;
	int yIn = path[0] % size_map;

	for (auto pix : path)
	{
		int x = pix / size_map;
		int y = pix % size_map;

		if (x < size_map-1 &&  x <= minX && map[(x + 1) * size_map + y].team_id != team_id)
		{
			xIn = x + 1;
			yIn = y;
			minX = x;
		}
		if (x > maxX) {
			maxX = x;
		}
		if (y < minY) {
			minY = y;
		}
		if (y > maxY) {
			maxY = y;
		}
	}

	if (minX != size_map && minX < maxX-1 && minY < maxY - 1) {
		fillColorRec(map, xIn, yIn, team_id);
	}
}

std::vector<int> compute_path(std::vector<Pixel>& map, int x, int y)
{
	std::vector<int> path;
	int visited = -1;
	while (visited != 0)
	{
		int index = x * size_map + y;
		path.push_back(index);
		int visited = map[index].visited - 1;

		// Right
		if (x < size_map - 1) {
			int ind = (x + 1) * size_map + y;
			if (map[ind].visited == visited) 
			{
				x = x + 1;
				continue;
			}
		}
		// left
		if (x > 1) {
			int ind = (x - 1) * size_map + y;
			if (map[ind].visited == visited)
			{
				x = x - 1;
				continue;
			}
		}
		// Down
		if (y < size_map - 1) {
			int ind = x * size_map + (y + 1);
			if (map[ind].visited == visited)
			{
				y = y + 1;
				continue;
			}
		}
		// Up
		if (y > 1 ) {
			int ind = x * size_map + (y - 1);
			if (map[ind].visited == visited)
			{
				y = y - 1;
				continue;
			}
		}
		// Path not found 
		return path;
	}

	return path;
}


void checkLoopRec(std::vector<Pixel>& map, int startIndex, int targetIndex, int otherTargetIndex, int team_id)
{
	int visit = 0;
	int x = startIndex / size_map;
	int y = startIndex % size_map;
	
	map[startIndex].visited = visit;
	
	std::queue < std::tuple<int, int> > queue_path;
	queue_path.push({x, y});

	while (!queue_path.empty())
	{
		auto p = queue_path.front();
		queue_path.pop();

		int x = std::get<0>(p);
		int y = std::get<1>(p);

		int index = x * size_map + y;
		visit = map[index].visited + 1;
		/*if (index == targetIndex && (otherTargetIndex == -1 || index == otherTargetIndex))
			return;*/

		// Right
		if (x < size_map - 1) {
			int index = (x + 1) * size_map + y;
			if (map[index].team_id == team_id && 
				map[index].visited == -1) {
				map[index].visited = visit;
				queue_path.push({ x + 1, y});
			}
		}
		// left
		if (x > 1) {
			int index = (x - 1) * size_map + y;
			if (map[index].team_id == team_id && 
				map[index].visited == -1) {
				map[index].visited = visit;
				queue_path.push({ x - 1, y});
			}
		}
		// Down
		if (y < size_map - 1) {
			int index = x * size_map + (y + 1);
			if (map[index].team_id == team_id && 
				map[index].visited == -1) {
				map[index].visited = visit;
				queue_path.push({ x, y + 1});
			}
		}
		// Up
		if (y > 1) {
			int index = x * size_map + (y - 1);
			if (map[index].team_id == team_id && 
				map[index].visited == -1) {
				map[index].visited = visit;
				queue_path.push({ x, y - 1});
			}
		}
	}

}

void checkLoop(std::vector<Pixel>& map, int x, int y, int team_id)
{
	int index = x * size_map + y;
	std::vector<int> targets;

	// Right
	if (x < size_map-1) {
		int index = (x+1) * size_map + y;
		if (map[index].team_id == team_id) {
			targets.push_back(index);
		}
	}
	// left
	if (x > 1) {
		int index = (x - 1) * size_map + y;
		if (map[index].team_id == team_id) {
			targets.push_back(index);
		}
	}
	// Down
	if (y < size_map-1) {
		int index = x * size_map + (y + 1);
		if (map[index].team_id == team_id) {
			targets.push_back(index);
		}
	}
	// Up
	if (y > 1) {
		int index = x * size_map + (y - 1);
		if (map[index].team_id == team_id) {
			targets.push_back(index);
		}
	}


	int target_count = targets.size();
	if (target_count >= 2 && target_count <= 4) {
		clear_visited(map);
		int other_target = -1;
		if (target_count == 3) {
			other_target = targets[2];
		}
		checkLoopRec(map, targets[0], targets[1], other_target, team_id);
		map[index].team_id = team_id;
		
		int xT = targets[1] / size_map;
		int yT = targets[1] % size_map;

		auto path = compute_path(map, xT, yT);

		std::vector<int> path2;
		if (target_count == 3) {
			int xT = other_target / size_map;
			int yT = other_target % size_map;

			path2 = compute_path(map, xT, yT);
		}

		if (path.size() >= 5) {
			path.push_back(index);
			fillColor(map, std::move(path), team_id);
		}
		if (path2.size() >= 5) {
			path2.push_back(index);
			fillColor(map, std::move(path2), team_id);
		}
	}
	
}

void updatePixels(std::vector<Pixel>& map, Vec2D position, int team_id)
{
	int x = static_cast<int> (position.m_x / size_pixel);
	int y = static_cast<int> (position.m_y / size_pixel);
	if (x >= 0 && x < size_map &&
		y >= 0 && y < size_map)
	{
		int index = x * size_map + y;

		if (map[index].team_id != team_id)
		{
			checkLoop(map, x, y, team_id);
			map[index].team_id = team_id;
		}
	}
}


struct Score
{
	int team1_pixel_count = 0;
	int team2_pixel_count = 0;
};


void computeScore(Score& score, std::vector<Pixel>& map) {
	for (auto pixel : map) {

		if (pixel.team_id == 1) {
			score.team1_pixel_count += 1;
		}
		if (pixel.team_id == 2) {
			score.team2_pixel_count += 1;
		}
	}
}


struct Team {
	std::vector<Unit> units;
	int unitIndex = 0;
};

int main()
{
	std::vector<Pixel> map;
	createMap(map);

	sf::RenderWindow window(sf::VideoMode(1200, 1200), "KAMI");


	float pixel_size = size_pixel - 1.0f;

	sf::RectangleShape pixels[3];
	sf::RectangleShape pixelWhite(sf::Vector2f(pixel_size, pixel_size));
	pixelWhite.setFillColor(sf::Color::White);
	pixels[0] = pixelWhite;

	sf::RectangleShape pixelRed(sf::Vector2f(pixel_size, pixel_size));
	pixelRed.setFillColor(sf::Color::Red);
	pixels[1] = pixelRed;

	sf::RectangleShape pixelBlue(sf::Vector2f(pixel_size, pixel_size));
	pixelBlue.setFillColor(sf::Color::Blue);
	pixels[2] = pixelBlue;

	float speed = 0.1f;
	Vec2D right(speed, 0.0);
	Vec2D left(-speed, 0.0);
	Vec2D up(0.0, -speed);
	Vec2D down(0.0, speed);


	Team teams[2];
	teams[0].units.push_back(Unit(Vec2D(50.0f, 50.0), 1, sf::Color::Red));
	teams[1].units.push_back(Unit(Vec2D(1200.0f, 800.0f), 2, sf::Color::Blue));


	Vec2D camera;

	float sizeX = 4.0f * 1768.0f;
	float sizeY = 1000.0f;

	sf::CircleShape shape_selected(10.0f);
	shape_selected.setFillColor(sf::Color::Green);


	bool tab_has_been_released = false;
	bool enter_has_been_released = false;

	while (window.isOpen())
	{
		window.clear();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
				tab_has_been_released = true;
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				enter_has_been_released = true;
			}
		}

		for (int i = 0; i < 2; i++) {
			teams[i].unitIndex = teams[i].unitIndex % teams[i].units.size();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			teams[0].units[teams[0].unitIndex].direction = left;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			teams[0].units[teams[0].unitIndex].direction = right;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			teams[0].units[teams[0].unitIndex].direction = down;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			teams[0].units[teams[0].unitIndex].direction = up;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && enter_has_been_released)
		{
			teams[0].unitIndex++;
			enter_has_been_released = false;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			teams[1].units[teams[1].unitIndex].direction = left;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			teams[1].units[teams[1].unitIndex].direction = right;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			teams[1].units[teams[1].unitIndex].direction = down;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			teams[1].units[teams[1].unitIndex].direction = up;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && tab_has_been_released)
		{
			teams[1].unitIndex++;
			tab_has_been_released = false;
		}


		for (int i = 0; i < 2; ++i) {

			for (auto& unit : teams[i].units) {

				unit.position = unit.position + unit.direction;

				if (unit.position.m_x < 0.0f) {
					unit.position.m_x = 0.0f;
				}
				if (unit.position.m_y < 0.0f) {
					unit.position.m_y = 0.0f;
				}
				if (unit.position.m_x > (size_map - 1) * size_pixel) {
					unit.position.m_x = (size_map - 1) * size_pixel;
				}
				if (unit.position.m_y > (size_map - 1) * size_pixel) {
					unit.position.m_y = (size_map - 1) * size_pixel;
				}
			}
		}

		/*if (camera.m_x + 1500 < position.m_x)
		{
			camera = camera + right;
		}

		if (camera.m_x + 200 > position.m_x)
		{
			camera = camera + left;
		}*/

		Vec2D size_shape(10.0f, 10.0f);

		for (auto pixel : map)
		{
			shapeSetPosition(pixels[pixel.team_id], pixel.position - camera);
			window.draw(pixels[pixel.team_id]);
		}

		for (int i = 0; i < 2; i++) {
			for (auto& unit : teams[i].units)
			{
				shapeSetPosition(unit.shape, unit.position - camera + size_shape);
				updatePixels(map, unit.position + size_shape, unit.team_id);
				window.draw(unit.shape);
			}
			shapeSetPosition(shape_selected, teams[i].units[teams[i].unitIndex].position - camera + size_shape);
			window.draw(shape_selected);
		}

		

		Score score;
		computeScore(score, map);
		int unit_per_pixel = 100;

		if (static_cast<int> (1 + score.team1_pixel_count / unit_per_pixel) > teams[0].units.size())
		{
			teams[0].units.push_back(Unit(Vec2D(50.0f, 50.0), 1, sf::Color::Red));
		}
		else if (static_cast<int> (1 + score.team1_pixel_count / unit_per_pixel) < teams[0].units.size())
		{
			teams[0].units.pop_back();
		}

		if (static_cast<int> (1 + score.team2_pixel_count / unit_per_pixel) > teams[1].units.size())
		{
			teams[1].units.push_back(Unit(Vec2D(1200.0f, 800.0f), 2, sf::Color::Blue));
		}
		else if (static_cast<int> (1 + score.team2_pixel_count / unit_per_pixel) < teams[1].units.size())
		{
			teams[1].units.pop_back();
		}


			
		
		window.display();
	}

	return 0;
}




