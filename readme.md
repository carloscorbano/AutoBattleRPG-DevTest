# Turn RPG system #

> * Kokku Software Engineer Applicant Test
> * Current Version: 1.0.4

----

### TODO List: ###

> * [v] This application is an auto-battle RPG, that has a "grid" with cells/tiles/boxes organized as a matrix.
> * [v] This grid provides the length for the x and y-axis.
> * [x] There is also a Character with a Name, Health, BaseDamage, DamageMultiplier, current cell/box, target, and an Index.
> * [v] Each team should have one move per turn (except when the move places the character in attack range of an opposing team character)
> * [v] The game should work with a "battlefield" of any size, including a non-square matrix, for example 6x6 or 6x10 tiles in the grid.
> * [x] Make sure all the variables in CHARACTER are engaged in a code feature.
> * [x] The game should inform the player when the battle is over and which team has been declared victorious.
> * [v] The battlefield should only be reprinted/redrawn if a player makes a move.
> * [v] Each character should look for a possible target and attack it when this is viable and if not, move closer into attack range.
> * [v] Each candidate must also implement one of the following extra features in the application, to be selected depending on their month of birth.
> * [v] Characters cannot be on the same tile at the same time.
> * [v] Extra based on birth month (August): The character can attack/walk in 8 directions;

----

### Changelog: ###

> version 1.0.4:

> > * Removed Character logic from `BattleField.h` and placed it in `Character.h`.
> > * Added to `Grid.h` two helper methods to get the index from lines and columns and another method to get the line and column from a given index.
> > * Correction of how length was spelled in `Grid.h`, changed also the for loop variables (i to y and j to x).
> > * Moved `DrawBattlefield()` from `Grid.h` to `BattleField.h`.
> > * `Character.h` method `StartTurn()` now receives `BattleField` instead of `Grid`.
> > * Changed `ocupied` variable from boolean to integer, it will now hold the index of a character (if is ocupied, if is not ocupied, then it will have -1 as value).
> > * Changed method return type in `Character.h` from bool to void (`TakeDamage()`).
> > * Added a random damageMultiplier in `Character.h`, it'll roll a random multiplier that is within the range of 0.01 to 1.00.
> > * Changed `WalkTo()` in variable to `Grid*` in `Character.h`.
> > * Created a Helper function in `Character.h` to calculate the distance.
> > * Changed `CheckCloseTargets()` to `CheckTargetIsWithinAttackRange()` in `Character.h`.
> > * Some of the requirements in the TODO list are completed.

> version 1.0.3:

> > * Improved input handling in `BattleField.cpp` (`Setup()` and `GetPlayerChoice()`).
> > * Created `Helper.h` to store helper methods.
> > * Changed `Types.h` class Types to namespace Types.
> > * Changed `Character.h` constructor to have all parameters to correct initialize class.
> > * Added to `Helper.h` a method that returns the attack range of the given class.
> > * Added energy to `Character.h` to control the state machine.
> > * Added to `Helper.h` a method that returns the energy cost of the given state.
> > * Added to `Helper.h` a method that returns the base status (health, damage and energy) of the given class.
> > * Created the definition of CharacterClass range (MINIMAL and MAX IDs) in `Types.h`.
> > * Fixed the grid creation, now it accepts the input data from player to create a custom grid size.
> > * Fixed the random method and moved it to `Helper.h`.
> > * Simplification of turn state (game state machine) in `Types.h` and `BattleField.h`.
> > * Added to `BattleField.h` a `OnGameEnd()` to handle the game end.
> > * Removed feature from version 1.0.2 that limits the size of the grid using 8 bits integers.
> > * Added to `Grid.h` a method to get a random grid location based on a quad (`GetRandomAvailableGridBoxInQuad()`).
> > * Changed in `BattleField.h` list to vector.


> version 1.0.2:

> > * Retarget solution to the `platform toolset = v143` (Visual Studio 2022).
> > * Removed `using namespace std`.
> > * Removed some included files that was including themselves.
> > * Cleaned spaghetti code.
> > * Changed `enum` to `enum class` in `types.h`
> > * Created a state machine to better control the game flow (created `GameState` and `CharacterTurnState` enum to control the states).
> > * Updated code public and private fields, changing variables to camelCase convention.
> > * Changed raw pointers to smart pointers.
> > * Added `<memory>` include to use smart pointers.
> > * Changed `shared_ptr` to `unique_ptr` in `BattleField.h`.
> > * Added bit flag to `Character.h` (to be able to check if is enemy or player character).
> > * Renamed `allPlayers` to `allCharacters` in `BattleField.h`.
> > * Deleted tracking characters variables from `BattleField.h`. It must be handle by the Character Class itself.
> > * Removed `switch` state on `BattleField.h` (on `GetPlayerChoice()`), changed the input to be handle in a while loop and a security check for invalid input data.
> > * Added a method to get the string representation of the enum `CharacterClass` in `Types.h`
> > * Removed `CreatePlayerCharacter()` and `CreateEnemyCharacter()` from inside of `GetPlayerChoice()` and inserted it in the game state machine.
> > * Added the choice to make the grid with the given size, added the definition to the limit in the `BattleField.h`, also changed the int type to 8 bits due the limit beeing 10x10 (100 tiles), any change in the limits, this may be changed as well (not beeing bigger than 255 tiles).
> > * Added a helper function (`ClearConsole()`) to `BattleField.h`, this helper function helps to clear the console.

> version 1.0.1:

> > * Correction of readme.md file (incorrect markdown data).

> version 1.0.0:

> > * Creation of a readme.md file to keep the version/project information
> > * Changes to the folder/files organization (added a source folder to hold files).
> > * Changed `includes` in .cpp files to match the new folder organization.
> > * Included a .gitignore file to the project main folder.
> > * Renamed file AutoBattleC++.cpp to main.cpp and moved it to the src root folder.

> version 0.0.0:
    
> > * Creation of the project.