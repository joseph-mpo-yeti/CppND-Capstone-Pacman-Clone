#include <string>
#include <SFML/Graphics.hpp>

namespace Assets
{
    namespace image
    {
        static sf::Vector2u PLAYER_TEX[] = {
            // ------ Direction based -------
            // up [0, 1, 2]
            {17, 9}, {17, 10}, {17, 11}, 
            // down [3, 4, 5]
            {17, 3}, {17, 4}, {17, 5},
            // left [6, 7, 8]
            {17, 6}, {17, 7}, {17, 8},
            // right [9, 10, 11]
            {17, 0}, {17, 1}, {17, 2},
            // ------ State based -------
            // waiting [12]
            {17, 9},
            // idle in order (up, down, left, right) [13...16]
            {17, 10}, {17, 4}, {17, 7}, {17, 1},
            // dead [17...28]
            {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {7, 8}, {7, 9}, {7, 10}, {7, 11}
        };

        static sf::Vector2u ENEMY_RED_TEX[] = {
            // up [0, 1]
            {0, 0}, {0, 1},  
            // down [2, 3]
            {0, 2}, {0, 3},
            // left [4, 5]
            {0, 4}, {0, 5},
            // right [6, 7]
            {0, 6}, {0, 7},
            // dead in order (up, down, left, right) [8, 9, 10, 11]
            {7, 6}, {7, 7}, {7, 8}, {7, 5},
            // vulnerable [ 12, 13]
            {0, 12}, {0, 13}
        };

        static sf::Vector2u ENEMY_BLUE_TEX[] = {
            // up [0, 1]
            {2, 0}, {2, 1},  
            // down [2, 3]
            {2, 2}, {2, 3},
            // left [4, 5]
            {2, 4}, {2, 5},
            // right [6, 7]
            {2, 6}, {2, 7},
            // dead in order (up, down, left, right)
            {7, 6}, {7, 7}, {7, 8}, {7, 5},
            // vulnerable
            {0, 12}, {0, 13}
        };

        static sf::Vector2u ENEMY_GREEN_TEX[] = {
            // up [0, 1]
            {4, 0}, {4, 1}, 
            // down [2, 3]
            {4, 2}, {4, 3},
            // left [4, 5]
            {4, 4}, {4, 5},
            // right [6, 7]
            {4, 6}, {4, 7},
            // dead in order (up, down, left, right)
            {7, 6}, {7, 7}, {7, 8}, {7, 5},
            // vulnerable
            {0, 12}, {0, 13}
        };

        static sf::Vector2u ENEMY_MAGENTA_TEX[] = {
            // up [0, 1]
            {5, 0}, {5, 1}, 
            // down [2, 3]
            {5, 2}, {5, 3},
            // left [4, 5]
            {5, 4}, {5, 5},
            // right [6, 7]
            {5, 6}, {5, 7},
            // dead in order (up, down, left, right)
            {7, 6}, {7, 7}, {7, 8}, {7, 5},
            // vulnerable
            {0, 12}, {0, 13}
        };

        
    } // namespace images

    namespace audio
    {
        
    } // namespace audio
    
} // namespace Assets
