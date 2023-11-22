#include "lab_m1/Game/game.h"

#include <vector>
#include <iostream>

#include "lab_m1/Game/transform2D.h"
#include "lab_m1/Game/object2D.h"

using namespace std;
using namespace m1;


Game::Game()
{

}


Game::~Game()
{

}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Game::VisualizationTransf2DUnif(const Game::LogicSpace& logicSpace, const Game::ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Game::SetViewportArea(const Game::ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void Game::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 1920;   // logic width
    logicSpace.height = 1080;  // logic height

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    length = 0.99f;

    // Compute coordinates of square center
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;
    time = 0;

    glm::vec3 background_corner = glm::vec3(corner.x - 200, corner.y - 200, 0);
    Mesh* black_rectangle = object2D::CreateSqr("black_rectangle", background_corner, logicSpace.width + 400, glm::vec3(0, 0, 0), true);
    AddMeshToList(black_rectangle);

    Mesh* square_green = object2D::CreateSqr("square_green", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square_green);

    Mesh* square_grey = object2D::CreateSqr("square_grey", corner, squareSide, glm::vec3(0.5, 0.5, 05), false);
    AddMeshToList(square_grey);

    Mesh* square_red = object2D::CreateSqr("square_red", corner, squareSide / 2, glm::vec3(1, 0, 0), true);
    AddMeshToList(square_red);

    Mesh* rectangle = object2D::CreateRectangle("rectangle", corner, squareSide / 2, squareSide / 4 * 14, glm::vec3(1, 0, 0), true);
    AddMeshToList(rectangle);

    Mesh* enemy_orange = object2D::Enemy("enemy_orange", corner, squareSide / 2.5, glm::vec3(1, 0.5, 0), glm::vec3(0, 1, 0), true);
    AddMeshToList(enemy_orange);

    Mesh* enemy_red = object2D::Enemy("enemy_red", corner, squareSide / 2.5, glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), true);
    AddMeshToList(enemy_red);

    Mesh* enemy_white = object2D::Enemy("enemy_white", corner, squareSide / 2.5, glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), true);
    AddMeshToList(enemy_white);

    Mesh* enemy_purple = object2D::Enemy("enemy_purple", corner, squareSide / 2.5, glm::vec3(1, 0.5, 1), glm::vec3(0, 1, 0), true);
    AddMeshToList(enemy_purple);

    Mesh* rhombus_orange = object2D::CreateRhombus("rhombus_orange", corner, squareSide / 3, glm::vec3(1, 0.5, 0), true);
    AddMeshToList(rhombus_orange);

    Mesh* rhombus_red = object2D::CreateRhombus("rhombus_red", corner, squareSide / 3, glm::vec3(1, 0, 0), true);
    AddMeshToList(rhombus_red);

    Mesh* rhombus_white = object2D::CreateRhombus("rhombus_white", corner, squareSide / 3, glm::vec3(1, 1, 1), true);
    AddMeshToList(rhombus_white);

    Mesh* rhombus_purple = object2D::CreateRhombus("rhombus_purple", corner, squareSide / 3, glm::vec3(1, 0.5, 1), true);
    AddMeshToList(rhombus_purple);

    Mesh* star_orange = object2D::CreateStar("star_orange", corner, squareSide / 4, glm::vec3(1, 0.5, 0), true);
    AddMeshToList(star_orange);

    Mesh* star_red = object2D::CreateStar("star_red", corner, squareSide / 4, glm::vec3(1, 0, 0), true);
    AddMeshToList(star_red);

    Mesh* star_white = object2D::CreateStar("star_white", corner, squareSide / 4, glm::vec3(1, 1, 1), true);
    AddMeshToList(star_white);

    Mesh* star_purple = object2D::CreateStar("star_purple", corner, squareSide / 4, glm::vec3(1, 0.5, 1), true);
    AddMeshToList(star_purple);

    // grey star - smaller, acts as mana
    Mesh* star_grey = object2D::CreateStar("star_grey", corner, squareSide / 6, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(star_grey);

    // grey star for money
    Mesh* star_grey_small = object2D::CreateStar("star_grey_small", corner, squareSide / 10, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(star_grey_small);

    // star on the ground - gold / yellow
    Mesh* star_yellow = object2D::CreateStar("star_yellow", corner, squareSide / 6, glm::vec3(1, 1, 0), true);
    AddMeshToList(star_yellow);

    // The game scene consists of several squares located on a grid with 3 lines and 3 columns
    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < 3; i++) {
            ground_squares.push_back(std::make_pair(squareSide / 4 * 3 + i * squareSide / 4 * 5, j * squareSide / 4 * 5));
        }
    }

    // Graphic interface for players
    // Create 4 squares at top left
    for (int i = 0; i < 4; i++) {
        air_squares.push_back(std::make_pair(squareSide / 4 + i * squareSide / 4 * 5, squareSide / 4 * 17));
    }

    rhombuses.clear();
}


void Game::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

}

void Game::MoveProjectiles(std::vector<Game::StarProjectile>& stars, float deltaTime) {
    // move and rotate stars
    for (int i = 0; i < stars.size(); i++) {
	    // reset model matrix
        stars[i].modelMatrix = glm::mat3(1);

        // update position
        stars[i].xi += stars[i].v * deltaTime * 20;

        // update angular step
        stars[i].angularStep += deltaTime * -2.5;

        // update model matrix
        stars[i].modelMatrix *= transform2D::Translate(stars[i].xi, stars[i].yi);

        // update model matrix for rotation
        stars[i].modelMatrix *= transform2D::Rotate(stars[i].angularStep);

        // if out of bound, remove star
        if (stars[i].xi > logicSpace.width) {
			stars.erase(stars.begin() + i);
		}
    }
}

void Game::PlaceStar(float x, float y, float v, float angular, int type) {
	Game::StarProjectile star = Game::StarProjectile();
	star.xi = x;
	star.yi = y;
	star.v = v;
	star.angularStep = angular;
	star.modelMatrix = glm::mat3(1);
    star.type = type;

	Game::stars.push_back(star);
}

void Game::PlaceRhombus(int index, int type, float x_corner, float y_corner) {
    Game::RhombusDefender rhombus = Game::RhombusDefender();
	
    rhombus.xi = Game::index_to_sqr[index].first;
    rhombus.yi = Game::index_to_sqr[index].second;
    
    rhombus.x_corner = x_corner;
    rhombus.y_corner = y_corner;

	rhombus.modelMatrix = glm::mat3(1);

    if (type > 3 || type < 0) {
        return;
    }

    rhombus.type = type % 4;
	rhombus.time = 0;
	rhombus.isScaling = false;
	rhombus.scale = 1;

    // set firing speed based on type
    if (rhombus.type == 0) {
        rhombus.firingSpeed = 3;
    }
    else if (rhombus.type == 1) {
		rhombus.firingSpeed = 2;
    }
    else if (rhombus.type == 2) {
		rhombus.firingSpeed = 1;
    }
    else if (rhombus.type == 3) {
		rhombus.firingSpeed = 1;
	}

    // set grid based on y coordinate
    if (rhombus.yi >= 0 && rhombus.yi < squareSide / 4 * 5) {
		rhombus.grid = 0;
	} else if (rhombus.yi >= squareSide / 4 * 5 && rhombus.yi < squareSide / 4 * 10) {
		rhombus.grid = 1;
	} else if (rhombus.yi >= squareSide / 4 * 10 && rhombus.yi < squareSide / 4 * 15) {
		rhombus.grid = 2;
	} else {
		return;
	}
	
     Game::rhombuses.push_back(rhombus);
}

void Game::PlaceHexagon(int grid, int type) {
    Game::HexagonAttacker hexagon = Game::HexagonAttacker();
	
    hexagon.xi = logicSpace.width + 50;
    hexagon.yi = Game::grid_x[grid];

    hexagon.grid = grid;
    // {0 - orange, 1 - red, 2 - white, 3 - purple}
	hexagon.type = type; 

    if (type > 3 || type < 0) {
		return;
	}

    // set speed and hp based on type
    if (hexagon.type == 0) {
        hexagon.v = 2 * Game::game_difficulty;
        hexagon.hp = 3;
    } else if (hexagon.type == 1) {
        hexagon.v = 3 * Game::game_difficulty;
        hexagon.hp = 3;
    } else if (hexagon.type == 2) {
        hexagon.v = 4 * Game::game_difficulty;
        hexagon.hp = 3;
    } else if (hexagon.type == 3) {
        hexagon.v = 4 * Game::game_difficulty;
        hexagon.hp = 3;
    }

	hexagon.modelMatrix = glm::mat3(1);
    // if size larger than 10, dont place
    if (Game::hexagons.size() > 10) {
		return;
	}
	Game::hexagons.push_back(hexagon);
}

void Game::PlaceMoney(float x, float y) {
    Game::StarMoney money = Game::StarMoney();
	money.x = x;
	money.y = y;
	Game::money.push_back(money);
}

void Game::Update(float deltaTimeSeconds)
{   
    glm::ivec2 resolution = window->GetResolution();

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);

    // compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    
    // add hexagons at random times and positions (grid)
    time += deltaTimeSeconds;
    time_money += deltaTimeSeconds;

    float random = rand() % 15;
    float random_money = rand() % 15;

    if (random < 14) {
        random = 14;
    }

    if (random_money < 14) {
		random_money = 14;
	}

    // add hexagon at random time
    if (time > random) {
		int grid = rand() % 3;
		int type = rand() % 4;
		PlaceHexagon(grid, type);
		time = 0;
	}

    // add money at random time
    if (time_money > random_money) {
        // location is random, but not on the ground squares
        float x = rand() % ((int)logicSpace.width / 2) + logicSpace.width / 2;
        float y = rand() % (int)logicSpace.height;
        
        PlaceMoney(x, y);
	    time_money = 0;
    }

    // money on the ground
    for (int i = 0; i < money.size(); i++) {
        glm::mat3 modelMatrixMoney = glm::mat3(1);

        modelMatrixMoney *= visMatrix * transform2D::Translate(money[i].x, money[i].y);

        RenderMesh2D(meshes["star_yellow"], shaders["VertexColor"], modelMatrixMoney);
    }

    // render hexagons
    for (int i = 0; i < hexagons.size(); i++) {
        // reset model matrix
		hexagons[i].modelMatrix = glm::mat3(1);

		// update position
		hexagons[i].xi -= hexagons[i].v * deltaTimeSeconds * 10;

		// update model matrix
		hexagons[i].modelMatrix *= transform2D::Translate(hexagons[i].xi, hexagons[i].yi);

		 //check if out of bounds
        if (hexagons[i].xi < 0) {
        	hexagons.erase(hexagons.begin() + i);
            player_hp--;

            if (player_hp == 0) {
                // game over
                cout << "game over" << endl;
                exit(0);
            }
        }
        bool isDead = false;
        
        //check if hit by star of the same type
        for (int j = 0; j < stars.size(); j++) {
            if (stars[j].xi > hexagons[i].xi - squareSide / 2 && stars[j].xi < hexagons[i].xi + squareSide / 2
                && stars[j].yi > hexagons[i].yi - squareSide / 2 && stars[j].yi < hexagons[i].yi + squareSide / 2
                && stars[j].type == hexagons[i].type) {
				// remove star
				stars.erase(stars.begin() + j);
				// decrease hp
				hexagons[i].hp--;
                if (hexagons[i].hp == 0) {
				    // remove hexagon
                    hexagons[i].isScaling = true;
                    
                    // hexagon is dead
                    isDead = true;
                }
				break;
			}
		}

        // check if hexagon is dead (scaling down)
        if (hexagons[i].isScaling) {
            			hexagons[i].modelMatrix = glm::mat3(1);
			hexagons[i].scale -= deltaTimeSeconds;

			// update model matrix for scaling in-place
			hexagons[i].modelMatrix *= transform2D::Translate(hexagons[i].xi, hexagons[i].yi);
			hexagons[i].modelMatrix *= transform2D::Scale(hexagons[i].scale, hexagons[i].scale);

			string color = Game::hexagon_mesh[hexagons[i].type % 4];

			RenderMesh2D(meshes[color], shaders["VertexColor"], visMatrix * hexagons[i].modelMatrix);
			
            if (hexagons[i].modelMatrix[0][0] < 0.1) {
                hexagons.erase(hexagons.begin() + i);
                continue;
            }
        }


        string color = Game::hexagon_mesh[hexagons[i].type % 4];
        RenderMesh2D(meshes[color], shaders["VertexColor"], visMatrix * hexagons[i].modelMatrix);
    }

    // Player HP and money
    for (int i = 0; i < player_hp; i++) {
        glm::mat3 modelMatrixHP = glm::mat3(1);
        modelMatrixHP *= visMatrix * transform2D::Translate(player_x + i * squareSide / 3 * 2, player_y + squareSide / 4);
        RenderMesh2D(meshes["square_red"], shaders["VertexColor"], modelMatrixHP);
    }

    // money (player's)
    for (int i = 0; i < player_money; i++) {
        glm::mat3 modelMatrixMoney = glm::mat3(1);
        modelMatrixMoney *= visMatrix * transform2D::Translate(player_x + i * squareSide / 5 * 2, player_y);
        RenderMesh2D(meshes["star_grey"], shaders["VertexColor"], modelMatrixMoney);
    }

    // hud rhombuses
    for (int i = 0; i < air_squares.size(); i++) {
        // get coordinates of square in a model matrix
        glm::mat3 modelMatrixRhombus = glm::mat3(1);

        string type = Game::rhombus_mesh[i % 4];

        modelMatrixRhombus *= transform2D::Translate(air_squares[i].first + squareSide / 2, air_squares[i].second + squareSide / 2);
        RenderMesh2D(meshes[type], shaders["VertexColor"], visMatrix * modelMatrixRhombus);

        // hud rhombus cost
        for (int j = 0; j < i % 4 + 1; j++) {
			glm::mat3 modelMatrixMoney = glm::mat3(1);
            modelMatrixMoney *= visMatrix * transform2D::Translate(air_squares[i].first + j * squareSide / 5, air_squares[i].second);
			RenderMesh2D(meshes["star_grey_small"], shaders["VertexColor"], modelMatrixMoney);
        }
    }

    // add stars
    time += deltaTimeSeconds;

    // star movement and draw
    MoveProjectiles(stars, deltaTimeSeconds);
    for (int i = 0; i < stars.size(); i++) {
		string color = Game::star_mesh[stars[i].type % 4];
        RenderMesh2D(meshes[color], shaders["VertexColor"], visMatrix * stars[i].modelMatrix);
    }

    // render held rhombus if dragging
    if (isDragging) {
        glm::mat3 modelMatrixRhombus = glm::mat3(1);
		modelMatrixRhombus *= transform2D::Translate(dragEndX, dragEndY);

        // get type of rhombus
        if (draggedType > 3 || draggedType < 0) {
			return;
		}
        string type = Game::rhombus_mesh[draggedType % 4];

		RenderMesh2D(meshes[type], shaders["VertexColor"], visMatrix * modelMatrixRhombus);
    }

    // render rhombuses
    for (int i = 0; i < rhombuses.size(); i++) {
        // check if hit by hexagon
        for (int j = 0; j < hexagons.size(); j++) {
            if (rhombuses[i].xi + squareSide / 2 > hexagons[j].xi - squareSide / 2 && rhombuses[i].xi + squareSide / 2 < hexagons[j].xi + squareSide / 2
                && rhombuses[i].yi + squareSide / 2 > hexagons[j].yi - squareSide / 2 && rhombuses[i].yi + squareSide / 2 < hexagons[j].yi + squareSide / 2) {
                // remove rhombus
                rhombuses[i].isScaling = true;
            }
        }

        // check if rhombus is scaling down - if so, scale it down
        if (rhombuses[i].isScaling) {
            rhombuses[i].modelMatrix = glm::mat3(1);
            rhombuses[i].scale -= deltaTimeSeconds;

            // update model matrix for scaling in-place
            rhombuses[i].modelMatrix *= transform2D::Translate(rhombuses[i].xi + squareSide / 2, rhombuses[i].yi + squareSide / 2);
            rhombuses[i].modelMatrix *= transform2D::Scale(rhombuses[i].scale, rhombuses[i].scale);

            string color = Game::rhombus_mesh[rhombuses[i].type % 4];

            RenderMesh2D(meshes[color], shaders["VertexColor"], visMatrix * rhombuses[i].modelMatrix);
            
            if (rhombuses[i].modelMatrix[0][0] < 0.1) {
				rhombuses.erase(rhombuses.begin() + i);
				break;
			}

            continue;
        } else {
            glm::mat3 modelMatrixRhombus = glm::mat3(1);
            modelMatrixRhombus *= transform2D::Translate(rhombuses[i].xi + squareSide / 2, rhombuses[i].yi + squareSide / 2);
            
            string color = Game::rhombus_mesh[rhombuses[i].type % 4];

            RenderMesh2D(meshes[color], shaders["VertexColor"], visMatrix * modelMatrixRhombus);
        }

        // increase time and check if it's time to fire (if not scaling)
        if (!rhombuses[i].isScaling) {
            rhombuses[i].time += deltaTimeSeconds;

            // check if there are any same type enemies on the same grid
            bool canFire = false;
            for (int j = 0; j < hexagons.size(); j++) {
                if (hexagons[j].grid == rhombuses[i].grid && hexagons[j].type == rhombuses[i].type) {
					canFire = true;
					break;
				}
			}

            if (rhombuses[i].time > rhombuses[i].firingSpeed && canFire) {
                PlaceStar(rhombuses[i].xi + squareSide / 2, rhombuses[i].yi + squareSide / 2, 25, 0.1, rhombuses[i].type);
                rhombuses[i].time = 0;
            }
            else if (rhombuses[i].time > rhombuses[i].firingSpeed) {
                rhombuses[i].time = rhombuses[i].firingSpeed;
            }
        }
    }

    // Game scene
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

    // Render Ground squares
    for (int i = 0; i < ground_squares.size(); i++) {
		// get coordinates of square in a model matrix
		glm::mat3 modelMatrixSquare = glm::mat3(1);
		modelMatrixSquare *= visMatrix * transform2D::Translate(ground_squares[i].first, ground_squares[i].second);

		// Render Green Square
		RenderMesh2D(meshes["square_green"], shaders["VertexColor"], modelMatrixSquare);
	}

    // Render Air squares
    for (int i = 0; i < air_squares.size(); i++) {
        // get coordinates of square in a model matrix
        glm::mat3 modelMatrixSquare = glm::mat3(1);
        modelMatrixSquare *= visMatrix * transform2D::Translate(air_squares[i].first, air_squares[i].second);

        // Render Grey Square
        RenderMesh2D(meshes["square_grey"], shaders["VertexColor"], modelMatrixSquare);
    }

    // Set black background
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(0, 0);
    RenderMesh2D(meshes["black_rectangle"], shaders["VertexColor"], visMatrix * modelMatrix);
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        // check if dragging started in the air squares
        for (int i = 0; i < air_squares.size(); i++) {
            if (dragStartX > air_squares[i].first && dragStartX < air_squares[i].first + squareSide
                && dragStartY > air_squares[i].second && dragStartY < air_squares[i].second + squareSide) {
				isDragging = true;
				draggedType = i % 4;

                dragEndX = (float)mouseX / window->GetResolution().x * logicSpace.width;
                dragEndY = (window->GetResolution().y - (float)mouseY) / window->GetResolution().y * logicSpace.height;

				break;
            }
		}
    } else {
	    isDragging = false;
	}
}


void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    int resolution_x = window->GetResolution().x;
    int resolution_y = window->GetResolution().y;

    dragStartX = (float)mouseX / resolution_x * logicSpace.width;
    dragStartY = (resolution_y - (float)mouseY) / resolution_y * logicSpace.height;

    // check if mouse clicked on money on the ground
    for (int i = 0; i < money.size(); i++) {
        if (dragStartX > money[i].x - squareSide / 6 && dragStartX < money[i].x + squareSide / 6
            && dragStartY > money[i].y - squareSide / 6 && dragStartY < money[i].y + squareSide / 6) {
            // remove money
            money.erase(money.begin() + i);

            // add money to player
            player_money++;
        }
    }

    // check if mouse clicked on a ground rhombus
    for (int i = 0; i < rhombuses.size(); i++) {
        if (dragStartX > rhombuses[i].xi && dragStartX < rhombuses[i].xi + squareSide
            && dragStartY > rhombuses[i].yi && dragStartY < rhombuses[i].yi + squareSide) {
			// remove rhombus - start scaling down
            rhombuses[i].isScaling = true;
		}
	}
}


void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // check if dragging
    if (isDragging) {
        // check if dragging started in the air squares
        for (int i = 0; i < air_squares.size(); i++) {
            if (dragStartX > air_squares[i].first && dragStartX < air_squares[i].first + squareSide
                && dragStartY > air_squares[i].second && dragStartY < air_squares[i].second + squareSide) {
                // check if dropped in a ground square
                for (int j = 0; j < ground_squares.size(); j++) {
                    if (dragEndX > ground_squares[j].first && dragEndX < ground_squares[j].first + squareSide
                        && dragEndY > ground_squares[j].second && dragEndY < ground_squares[j].second + squareSide) {
                        // check if player has enough money
                        if (player_money < i + 1) {
                            cout << "not enough money" << endl;
                            break;
                        }

                        // check if there is already a rhombus there
                        for (int k = 0; k < rhombuses.size(); k++) {
                            if (ground_squares[j].first == rhombuses[k].xi && ground_squares[j].second == rhombuses[k].yi) {
								cout << "already placed" << endl;
								break;
							}
						}

                        for (int p = 0; p < ground_squares.size(); p++) {
                            if (dragEndX > ground_squares[p].first && dragEndX < ground_squares[p].first + squareSide
                                && dragEndY > ground_squares[p].second && dragEndY < ground_squares[p].second + squareSide) {
           					    // place rhombus
                                PlaceRhombus(p, i, ground_squares[p].first, ground_squares[p].second);
                                player_money -= i + 1;
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    isDragging = false;
}


void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{

}


void Game::OnWindowResize(int width, int height)
{

}


void Game::FrameEnd()
{

}


void Game::OnInputUpdate(float deltaTime, int mods)
{

}


void Game::OnKeyPress(int key, int mods)
{

}


void Game::OnKeyRelease(int key, int mods)
{

}
