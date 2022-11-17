#include "Entity.h"
#include "QuadTree.h"

sf::RenderWindow window;

std::vector<std::shared_ptr<Entity>> entities;
QuadTree qt{ sf::FloatRect{ 0U, 0U, 800U, 600U }, 15U, 1U };

void init()
{
    window.create( sf::VideoMode{ 800U, 600U }, "Quad Tree" );
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(10, 700), disY(10, 500),
        vel(5, 150);
    for(size_t i = 0; i < 100; ++i) {
        entities.emplace_back(std::make_shared<Entity>(
            sf::Vector2f{25 + (float)disX(gen), 25 + (float)disY(gen)}));
        entities.back()->setVelocity({50 + (float)vel(gen), 50 + (float)vel(gen)});
    }
    qt.insert(entities);
}

void handleEvents(sf::Event const& e)
{
    switch(e.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if(!entities.empty())
                entities.erase(entities.end());
            break;
        default:
            break;
    }
}

void handleCollisions()
{
    for(size_t i{ 0 }; i < entities.size() - 1; ++i) {
        auto tmp{ qt.getCollisionable(entities[i]->getGlobalBounds()) };
        if(tmp.size() > 1) {
            for(size_t j{ 0 }; j < tmp.size(); ++j) {
                if((tmp[j].lock().get()) == (entities[i].get()))
                    continue;
                if(tmp[j].lock()->getGlobalBounds().intersects(entities[i]->getGlobalBounds())) {
                    tmp[j].lock()->setColor(sf::Color::Magenta);
                    entities[i]->setColor(sf::Color::Magenta);
                }
            }
        }
    }
}

void update(sf::Time const& dt)
{
    for(auto& entity : entities)
        entity->setColor(sf::Color::Red);
    for(auto& entity : entities) {
        entity->update(dt);
        if(!QuadTreeNode::contains({ 0U, 0U, 800U, 600U }, entity->getGlobalBounds()))
            entity->setVelocity({entity->getVelocity().y, -entity->getVelocity().x});
    }
    qt.clear();
    qt.insert(entities);
    handleCollisions();
}

void render()
{
    window.clear(sf::Color::Black);
    for(auto const& entity : entities)
        window.draw(*entity);
    qt.drawBorders(window);
    window.display();
}

int main()
{
    init();

    sf::Event e;
    sf::Clock frameClock;
    sf::Time elapsedTime{ sf::Time::Zero };
    sf::Time TimePerFrame{ sf::seconds(1.f / 60.f) };
    while(window.isOpen()) {
        elapsedTime += frameClock.restart();
        while(window.pollEvent(e))
            handleEvents(e);
        while(elapsedTime > TimePerFrame)
        {
            elapsedTime -= TimePerFrame;
            update(TimePerFrame);
        }

        render();
    }

    return EXIT_SUCCESS;
}
