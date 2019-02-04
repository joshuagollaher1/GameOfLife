#include <stdio.h>
#include <cstdlib>
#include <exception>
#include <memory.h>
#include <SFML/Graphics.hpp>
#include <iostream>

//Conways game of life in c++

constexpr int WIDTH = 80;
constexpr int HEIGHT = 80;
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

float CELL_WIDTH = WINDOW_WIDTH / WIDTH;
float CELL_HEIGHT = WINDOW_HEIGHT / HEIGHT;

struct Cells
{
	std::array<bool, WIDTH*HEIGHT> cells;
	
	Cells(const Cells& other)
	{
		cells = other.cells;
	}
	
	Cells()
	{
		for(int i = 0; i < WIDTH*HEIGHT; i++)
		{
			this->cells[i] = false;
		}
	}
	
	void SetCellAt(size_t x, size_t y, bool value, bool throwError = false)
	{
		if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		{
			if(throwError)
			{
				throw std::exception();
			}
			else
			{
				return;
			}
		}
		this->cells[WIDTH * y + x] = value;
	}
	
	bool GetCellAt(size_t x, size_t y, bool throwError = false)
	{
		if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		{
			if(throwError)
			{
				throw std::exception();
			}
			else
			{
				return false;
			}
		}
		return this->cells[WIDTH * y + x];
	}
	
	void LogCells()
	{
		std::cout << "Cells:" << std::endl;
		for(int j = 0; j < HEIGHT; j++)
		{
			for(int i = 0; i < WIDTH; i++)
			{
				std::cout << (this->GetCellAt(i, j) ? "1" : "0") << ", ";
			}
			std::cout << std::endl;
		}
		std::cout << "--------------" << std::endl;
	}
};

int neighbors(int x, int y, Cells cells)
{
	int n = 0;
	n += (int)cells.GetCellAt(x - 1, y); //W
	n += (int)cells.GetCellAt(x - 1, y - 1); //NW
	n += (int)cells.GetCellAt(x, y - 1); //N
	n += (int)cells.GetCellAt(x + 1, y - 1); //NE
	n += (int)cells.GetCellAt(x + 1, y); //E
	n += (int)cells.GetCellAt(x + 1, y + 1); //SE
	n += (int)cells.GetCellAt(x, y + 1); //S
	n += (int)cells.GetCellAt(x - 1, y + 1); //SW
	
	return n;
}

Cells one_cycle(Cells cells)
{
	Cells result;
	result.cells = cells.cells;
	
	for(int i = 0; i < WIDTH; i++)
	{
		for(int j = 0; j < HEIGHT; j++)
		{
			 int n = neighbors(i, j, cells);
			 if(cells.GetCellAt(i, j))
			 {
				if(n <= 1)
				{
					result.SetCellAt(i, j, false);
				}
				else if(n == 2 || n == 3)
				{
					result.SetCellAt(i, j, true);
				}
				else if(n >= 4)
				{
					result.SetCellAt(i, j, false);
				}
			 }
			 else
			 {
				 if(n == 3)
				 {
					 result.SetCellAt(i, j, true);
				 }
			 }
		}
	}
	return result;
}

void render_cells(Cells &cells, sf::RenderWindow &window)
{
	for(int i = 0; i < WIDTH; i++)
	{
		for(int j = 0; j < HEIGHT; j++)
		{
			if(cells.GetCellAt(i, j))
			{
				
				//render it
				sf::RectangleShape rs(sf::Vector2f(CELL_WIDTH, CELL_HEIGHT));
				rs.setPosition(sf::Vector2f(CELL_WIDTH * i, CELL_HEIGHT * j));
				
				//std::cout << "Rendering cell at " << CELL_WIDTH * i << ", " << CELL_WIDTH * j << std::endl;
				
				window.draw(rs);
			}
		}
	}
}

void initial_pattern(Cells &cells)
{
	cells.SetCellAt(4, 4, true);
	cells.SetCellAt(5, 5, true);
	cells.SetCellAt(5, 6, true);
	cells.SetCellAt(4, 6, true);
	cells.SetCellAt(3, 6, true);
}

int main(int argc, char **argv)
{
	Cells cells;
	
	sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
	sf::Clock clock;
	float delta_time;
	
	float fps = 4;
	float waiting_time = 1 / fps;
	float current_time = 0;
	
	initial_pattern(cells);
	
	while(window.isOpen())
	{
		delta_time = clock.restart().asSeconds();
		
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		
		current_time += delta_time;
		if(current_time >= waiting_time)
		{
			current_time = 0;
			
			cells = one_cycle(cells);
		}
		
		window.clear();
		
		render_cells(cells, window);
		
		window.display();
	}
	
	return 0;
}
