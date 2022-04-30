# Turn RPG system #

> * Kokku Software Engineer Applicant Test
> * Current Version: 1.2.0

----

### TODO List: ###

> * [v] This application is an auto-battle RPG, that has a "grid" with cells/tiles/boxes organized as a matrix.
> * [v] This grid provides the length for the x and y-axis.
> * [v] There is also a Character with a Name, Health, BaseDamage, DamageMultiplier, current cell/box, target, and an Index.
> * [v] Each team should have one move per turn (except when the move places the character in attack range of an opposing team character)
> * [v] The game should work with a "battlefield" of any size, including a non-square matrix, for example 6x6 or 6x10 tiles in the grid.
> * [v] Make sure all the variables in CHARACTER are engaged in a code feature.
> * [v] The game should inform the player when the battle is over and which team has been declared victorious.
> * [v] The battlefield should only be reprinted/redrawn if a player makes a move.
> * [v] Each character should look for a possible target and attack it when this is viable and if not, move closer into attack range.
> * [v] Each candidate must also implement one of the following extra features in the application, to be selected depending on their month of birth.
> * [v] Characters cannot be on the same tile at the same time.
> * [v] Extra based on birth month (August): The character can attack/walk in 8 directions;

----

### Changelog: ###

> version 1.2.0:

> > * Major update -> Starting to implement visual feedback.
> > * Added a `Setup()` in `Console.h` to setup the console and a KeyState to retrieve if the key was pressed.
> > * Added a `DrawToScreen()` in `Console.h` to handle all the drawing.
> > * Added base controls to the game.
> > * Added a input handler in `Console.h`.
> > * Added UI states in GameState.
> > * Started to draw scene UI.

> version 1.1.1:

> > * Changed `GetAllBoxesAroundGridBoxQuadSearch()` and `GetAllBoxesAroundGridBoxCircleSearch()` to accept the return of the centre box as well.
> > * Added a wait for seconds in the AI state machine, to delay it (it was too fast before).
> > * Added to `Character.h` a `DecideAction()` and to the enum as well `CharacterTurnState::DecidingAction`.
> > * Creation of the `Console` class (starting to work on the UI).
> > * Changed `DrawBattleField()` to `UpdateBattleField()` in `BattleField.h`.

> version 1.1.0:

> > * Creation of struct `CharacterInfo` in `Types.h` to better code organization, adapted `Character.h` to receive this new struct.
> > * Created `Defs.h` to hold all the definitions in one place.
> > * Changed `Character.h` constructor to accept the new struct.
> > * Created `SpawnCharacters()` in `BattleField.h`.
> > * Removed all getters and setters from `Character.h`.
> > * Added one getter to `Character.h` to retrieve `CharacterInfo`.
> > * Changed method of character creation, now splited into two sections (one is to create the character information and object, the second is to spawn the character).
> > * Changed the return type from `int` to `Types::GridBox` and renamed the method from `GetRandomAvailableIndexInQuad()` to `GetRandomAvailableGridInQuad()` in `Grid.h`.
> > * Moved GameEnd handling logic to inside of `OnGameEnd()` in `BattleField.h`.
> > * Added `GetAllBoxesAroundGridBoxQuadSearch()` and `GetAllBoxesAroundGridBoxCircleSearch()` to `Grid.h` (a helper function to fast check for objects).
> > * Moved `CalculateDistance()` from `Character.h` to `Helper.h` and renamed to `CalculateDistManhattan()`.
> > * Correction of how `ocupied` was spelled to `occupied`. 
> > * Added a safeguard code to prevent out of index search in `Grid.h`
> > * Added two helper methods to `BattleField.h` to read input from user.
> > * Correction of the text that was printed to the user, now it's easier to read.

> version 1.0.5:

> > * Optimization of `BattleField.h` and `Character.h`, removed the need for search for player icon within all the players, it now holds the index of the vector.
> > * Added a play again selection after the game is over.
> > * Added a winner flag to `BattleField.h`.
> > * Added a field `Name` in `Character.h`.
> > * Added a method in `Helper.h` to get random names.
> > * Fixed some editor warnings (size_t cast warnings).
> > * Cleaned `BattleField.h`, removing some unused methods and old code.
> > * Added a method to wait for seconds in `Helper.h`.
> > * Removed the need for press key between turns, now the game runs until the end (using the wait for seconds method to avoid beeing too fast).
> > * Cleaned `Character.h`, removing some unused old code.
> > * Added some summary data for the `Helper.h` methods.
> > * Added a destructor to `BattleField.h`.
> > * Cleaned some spaces and empty lines from all files.

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