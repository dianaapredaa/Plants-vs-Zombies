#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Game : public gfxc::SimpleScene
    {
    public:
        Game();
        ~Game();

        typedef struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        }ViewportSpace;

        typedef struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        }LogicSpace;

        void Init() override;

        float length;

        float squareSide = 200;
        glm::vec3 corner = glm::vec3(0, 0, 0);
        glm::mat3 Game::VisualizationTransf2DUnif(const Game::LogicSpace& logicSpace, const Game::ViewportSpace& viewSpace);
        void Game::SetViewportArea(const Game::ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear);
        struct ViewportSpace viewSpace;
        struct LogicSpace logicSpace;
        glm::mat3 visMatrix;
        typedef struct StarProjectile {
            // initial position
            float xi, yi;

            // model matrix for position
            glm::mat3 modelMatrix;

            // velocity
            float v;

            // color
            glm::vec3 color;

            int type;

            // angular step
            float angularStep;

        }StarProjectile;

        typedef struct RhombusDefender {
            float xi, yi;
            glm::mat3 modelMatrix;
            int type;
            float firingSpeed;
            bool isScaling = false;
            float scale = 1;
            float time = 0;
            float x_corner = 0;
            float y_corner = 0;
            int grid = 0;
        }RhombusDefendes;

        std::vector<std::string> rhombus_mesh = { "rhombus_orange", "rhombus_red", "rhombus_white", "rhombus_purple"};
        std::vector<std::string> hexagon_mesh = { "enemy_orange", "enemy_red", "enemy_white", "enemy_purple" };
        std::vector<std::string> star_mesh = { "star_orange", "star_red", "star_white", "star_purple" };

        typedef struct HexagonAttacker {
            float xi, yi;
            glm::mat3 modelMatrix;
            float v;
            int grid;
            int hp;
            int type;
            float scale = 1;
            bool isScaling = false;
        };

        typedef struct StarMoney {
            float x, y;
        };

        // y = j * squareSide / 4 * 5
        float square_top = 0 + squareSide / 2;
        float square_mid = squareSide / 4 * 5 + squareSide / 2;
        float square_bot = squareSide / 4 * 10 + squareSide / 2;


        std::vector<float> grid_x = { square_top, square_mid, square_bot };

        void Game::MoveProjectiles(std::vector<StarProjectile>& stars, float deltaTime);
        void Game::PlaceStar(float x, float y, float v, float angular, int type);

        // stars
        std::vector<StarProjectile> stars;

        // rhombuses fixed size - 9 squares on the ground
        std::vector<RhombusDefender> rhombuses;

        // hexagons
        std::vector<HexagonAttacker> hexagons;

        // money on the ground
        std::vector<StarMoney> money;

        void Game::PlaceRhombus(int index, int type, float x_corner, float y_corner); // 0 - top left square, 8 - bottom right square
        void Game::PlaceHexagon(int grid, int type);
        void Game::PlaceMoney(float x, float y);

        // placeable squares
        std::vector<std::pair<float, float>> ground_squares;
        std::vector<std::pair<float, float>> air_squares;
        bool isDragging;
        int draggedType = 0;
        float dragStartX, dragStartY;
        float dragEndX, dragEndY;

        //     for (int j = 0; j < 3; ++j) {
             /*   for (int i = 0; i < 3; i++) {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(squareSide / 4 * 3 + i * squareSide / 4 * 5, j * squareSide / 4 * 5);

                    ground_squares.push_back(std::make_pair(squareSide / 4 * 3 + i * squareSide / 4 * 5, j * squareSide / 4 * 5));

                    RenderMesh2D(meshes["square_green"], shaders["VertexColor"], visMatrix * modelMatrix);
                }
            }*/
        std::unordered_map<int, std::pair<float, float>> index_to_sqr = {
            {0, std::make_pair(squareSide / 4 * 3, 0)},
            {1, std::make_pair(squareSide / 4 * 8, 0)},
            {2, std::make_pair(squareSide / 4 * 13, 0)},
            {3, std::make_pair(squareSide / 4 * 3, squareSide / 4 * 5)},
            {4, std::make_pair(squareSide / 4 * 8, squareSide / 4 * 5)},
            {5, std::make_pair(squareSide / 4 * 13, squareSide / 4 * 5)},
            {6, std::make_pair(squareSide / 4 * 3, squareSide / 4 * 10)},
            {7, std::make_pair(squareSide / 4 * 8, squareSide / 4 * 10)},
            {8, std::make_pair(squareSide / 4 * 13, squareSide / 4 * 10)}
        };

        std::unordered_map<float, int> sqr_to_x = {
            {squareSide / 4 * 3, 0},
            {squareSide / 4 * 8, 1},
            {squareSide / 4 * 13, 2},
            {squareSide / 4 * 3, 3},
            {squareSide / 4 * 8, 4},
            {squareSide / 4 * 13, 5},
            {squareSide / 4 * 3, 6},
            {squareSide / 4 * 8, 7},
            {squareSide / 4 * 13, 8}
        };
        std::unordered_map<float, int> sqr_to_y = {
            {0, 0},
            {squareSide / 4 * 5, 1},
            {squareSide / 4 * 10, 2},
            {0, 3},
            {squareSide / 4 * 5, 4},
            {squareSide / 4 * 10, 5},
			{0, 6},
            {squareSide / 4 * 5, 7},
            {squareSide / 4 * 10, 8}
		};

        int player_hp = 3;
        int player_money = 5;

        float player_x = 1200;
        float player_y = 810;

        float game_difficulty = 1;

        float time = 0;
        float time_money = 0;

    private:

        
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;

    };

}   // namespace m1

// TODO:
// Add money when clicking on a money bag
// FIX drag and drop!!!
// URGENT: FIX CRASH ??? SUBSCRIPT OUT OF RANGE i think done