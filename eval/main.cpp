// réalisé par Pierre Saugues et Lina Smati

#include <iostream>
#include <deque>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>
#include <ctime>

class Point
{
    int x;
    int y;

public:
    Point(int xCoord, int yCoord) : x(xCoord), y(yCoord)
    {
    }

    void display() const
    {
        std::cout << "Point(" << x << ", " << y << ")" << std::endl;
    }

    int getX() const { return x; }
    int getY() const { return y; }

    bool operator<(const Point& other) const
    {
        return (x > other.x) || (x == other.x && y > other.y);
    }

    bool operator>(const Point& other) const
    {
        return (x > other.x) || (x == other.x && y > other.y);
    }
};

struct Cell
{
    bool passable;
    double cost;
    bool isPath;

    Cell(bool passable = false, double cost = 1, bool isPath = false) : passable(passable), cost(cost), isPath(isPath)
    {
    }
};

using Deque = std::deque<std::deque<Cell>>;

Deque createDeque(int rows, int cols)
{
    Deque tableau(rows);

    std::srand(std::time(nullptr));

    for (int i = 0; i < rows; ++i)
    {
        std::deque<Cell> row;
        for (int j = 0; j < cols; ++j)
        {
            bool passable = (std::rand() % 100) > 10;
            double cost = 1;
            row.push_back(Cell(passable, cost));
        }
        tableau[i] = row;
    }

    return tableau;
}

void printDeque(const Deque& tableau)
{
    for (const auto& row : tableau)
    {
        for (const auto& cell : row)
        {
            if (cell.isPath)
            {
                std::cout << "- ";
            }
            else if (cell.passable)
            {
                std::cout << "* ";
            }
            else
            {
                std::cout << "x ";
            }
        }
        std::cout << std::endl;
    }
}

void addRow(Deque& tableau, int cols, bool top = false)
{
    std::srand(std::time(nullptr));

    std::deque<Cell> rowToAdd(cols);
    for (int i = 0; i < cols; ++i)
    {
        bool passable = (std::rand() % 100) > 10;
        double cost = 1;
        rowToAdd[i] = Cell(passable, cost);
    }

    if (top)
    {
        tableau.push_front(rowToAdd);
    }
    else
    {
        tableau.push_back(rowToAdd);
    }
}

void addCol(Deque& tableau, bool left)
{
    std::srand(std::time(nullptr));

    for (auto& row : tableau)
    {
        bool passable = (std::rand() % 100) > 10;
        double cost = 1;
        if (left)
        {
            row.push_front(Cell(passable, cost));
        }
        else
        {
            row.push_back(Cell(passable, cost));
        }
    }
}

void dijkstra(Deque& tableau, Point* depart, Point* arrive)
{
    int rows = tableau.size();
    int cols = tableau[0].size();
    std::vector dist(rows, std::vector(cols, std::numeric_limits<double>::max()));
    std::vector prev(rows, std::vector(cols, Point(-1, -1)));
    std::priority_queue<Point, std::vector<Point>, std::greater<Point>> pq;

    int startX = depart->getX();
    int startY = depart->getY();
    int endX = arrive->getX();
    int endY = arrive->getY();

    dist[startX][startY] = 0;
    pq.emplace(*depart);

    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    while (!pq.empty())
    {
        Point current = pq.top();
        pq.pop();

        int x = current.getX();
        int y = current.getY();

        if (x == endX && y == endY)
        {
            break;
        }

        for (const auto& dir : directions)
        {
            int newX = x + dir.first;
            int newY = y + dir.second;
            double newCost = (std::abs(dir.first) + std::abs(dir.second) == 2) ? 1.41 : 1.0;

            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && tableau[newX][newY].passable)
            {
                double newDist = dist[x][y] + newCost;
                if (newDist < dist[newX][newY])
                {
                    dist[newX][newY] = newDist;
                    prev[newX][newY] = Point(x, y);
                    pq.emplace(Point(newX, newY));
                }
            }
        }
    }

    if (prev[endX][endY].getX() == -1)
    {
        std::cout << "Point d'arrivée non atteignable." << std::endl;
        return;
    }

    Point p = *arrive;
    while (prev[p.getX()][p.getY()].getX() != -1)
    {
        tableau[p.getX()][p.getY()].isPath = true;
        p = prev[p.getX()][p.getY()];
    }
    tableau[startX][startY].isPath = true;
}


int main()
{
    int rows = 20;
    int cols = 20;
    Point* depart = new Point(0, 0);
    Point* arrive = new Point(3, 3);

    Deque deques = createDeque(rows, cols);

    deques[0][1].passable = true;
    deques[2][3].passable = true;

    addCol(deques, true);
    addRow(deques, deques[0].size(), true);

    dijkstra(deques, depart, arrive);

    printDeque(deques);

    delete depart;
    delete arrive;

    return 0;
}