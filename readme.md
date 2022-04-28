# Turn RPG system #

> * Kokku Software Engineer Applicant Test
> * Current Version: 1.0.2

----

### TODO List: ###

> * [x] This application is an auto-battle RPG, that has a "grid" with cells/tiles/boxes organized as a matrix.
> * [x] This grid provides the length for the x and y-axis.
> * [x] There is also a Character with a Name, Health, BaseDamage, DamageMultiplier, current cell/box, target, and an Index.
> * [x] Each team should have one move per turn (except when the move places the character in attack range of an opposing team character)
> * [x] The game should work with a "battlefield" of any size, including a non-square matrix, for example 6x6 or 6x10 tiles in the grid.
> * [x] Make sure all the variables in CHARACTER are engaged in a code feature.
> * [x] The game should inform the player when the battle is over and which team has been declared victorious.
> * [x] The battlefield should only be reprinted/redrawn if a player makes a move.
> * [x] Each character should look for a possible target and attack it when this is viable and if not, move closer into attack range.
> * [x] Each candidate must also implement one of the following extra features in the application, to be selected depending on their month of birth.
> * [x] Characters cannot be on the same tile at the same time.
> * [x] Extra based on birth month (August): The character can attack/walk in 8 directions;

----

### Changelog: ###

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