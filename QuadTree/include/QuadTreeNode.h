#ifndef QUADTREENODE_H_
#define QUADTREENODE_H_

#include "Common.h"
#include "Entity.h"

class QuadTree;

class QuadTreeNode : private NonCopyable
{
        friend class QuadTree;
    public:
        explicit QuadTreeNode(sf::FloatRect bounds, size_t level,
                              size_t const& maxLevel, size_t const& maxObjects);
        size_t countObjects() const;
        static bool contains(sf::FloatRect const& first, sf::FloatRect const& second);
    private:
        void insert(std::weak_ptr<Entity> object);
        void drawBorders(sf::RenderTarget& t) const;
        void subdivide();
        void clear();
        size_t getIndex(sf::FloatRect objBounds) const;
        std::vector<std::weak_ptr<Entity>> getCollisionable(sf::FloatRect const& bounds) const;
        std::vector<std::weak_ptr<Entity>> getAllObjects() const;
        sf::FloatRect mBounds;
        size_t const mLevel;
        size_t const& mMaxLevel, mMaxObjects;
        QuadTreeNode* mParent{ nullptr };
        std::vector<std::weak_ptr<Entity>> mObjects{ };
        std::vector<std::weak_ptr<Entity>> mStuckObjects{ };
        enum Position { NW = 0, NE = 1, SW = 2, SE = 3 };
        std::array<std::unique_ptr<QuadTreeNode>, 4> mChildren{ { nullptr, nullptr, nullptr, nullptr } };
        bool isSubdivided{ false };
};

#endif
