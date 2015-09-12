#include "Engine.h"

Engine::Engine(int scrHeight, int scrWidth, bool fullScreen)
{

    EngineError = false;
    _scrHeight = scrHeight;
    _scrWidth = scrWidth;

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Engine Constructor: Unable to init SDL: %s\n", SDL_GetError() );
        EngineError = true;
    }

    //set title bar text
    std::string titleBarText = PROGRAM_NAME + " " + VERSION;
    SDL_WM_SetCaption(titleBarText.c_str() , titleBarText.c_str() );

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    if (fullScreen)
        _screen = SDL_SetVideoMode(_scrHeight, _scrWidth, 16, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
    else
        _screen = SDL_SetVideoMode(_scrHeight, _scrWidth, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);

    if ( !_screen )
    {
        printf("Engine Constructor: Unable to set %d by %d video: %s\n", _scrHeight, _scrWidth, SDL_GetError());
        EngineError = true;
    }

    //fonts initialization
    if (TTF_Init() == -1)
    {
        printf("Engine Constructor: Error initializing fonts: %s\n", TTF_GetError());
        EngineError = true;
    }

    _font = TTF_OpenFont("Helvetica.ttf", 20);
    if (NULL == _font)
    {
        printf("Engine Constructor: Unable to load font: %s\n%s\n", FONT_FILE_NAME.c_str(), TTF_GetError());
        EngineError = true;
    }

    //Clean up TTF on exit
    atexit(TTF_Quit);

    setUpAreaData();

    initializeSprites();

    clipTiles();

    if (!setUpTiles()){
        printf("Engine Constructor: Unable to set up tiles for map.\n");
        EngineError = true;
    }

}

Engine::~Engine()
{
    SDL_FreeSurface(_screen);
    TTF_CloseFont(_font);


    for (int i = 0; i < NUM_MESSENGERS; i++)
        delete(_msgr[i]);
    for (int i = 0; i < TOTAL_TILE_SPRITES; i++)
        delete(_clips[i]);
    for (int i = 0; i < NUM_TILES; i++)
        delete(_tiles[i]);

//    delete [] _item;
    delete(_player);
    delete(_testSprite);
    delete(_tileSheet);
}

void Engine::ClearScreen()
{
    // clear screen
    SDL_FillRect(_screen, 0, SDL_MapRGB(_screen->format, 0, 0, 0));
}

void Engine::RenderSprites()
{
    //draw projectile if visible
    if (_player->Weapon->Visible)
        SDL_BlitSurface(_player->Weapon->GetSprite(), 0, _screen, _player->Weapon->GetDestinationRect());

    //draw all items to the screen:
    for (int i = 0; i < _areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfItems(); i++){
        InventoryItem *tempItem = _areaData[_player->MapXLocation][_player->MapYLocation]->InventoryData->GetItem(i);

        if (NULL != tempItem){
            if (tempItem->Visible){
                SDL_BlitSurface(tempItem->GetSprite(), 0, _screen, tempItem->GetDestinationRect());
            }
        }
    }

    //draw player
    SDL_BlitSurface(_player->GetSprite(), 0, _screen, _player->GetDestinationRect());

    //update player health string
    _msgr[HEALTH]->SetSprite(TTF_RenderText_Solid(_font, _player->HealthString(), _msgr[HEALTH]->TextColor), 10, PLAYFIELD_HEIGHT + 45);

    //blit all visible messengers
    for (int i = 0; i < NUM_MESSENGERS; i++)
    {
        if (_msgr[i]->Visible)
            SDL_BlitSurface(_msgr[i]->GetSprite(), 0, _screen, _msgr[i]->GetDestinationRect());
    }

    //draw all enemies on the screen
    for (int i = 0; i < _areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfBadGuys(); i++)
        SDL_BlitSurface(_areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->GetSprite(), 0, _screen, _areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->GetDestinationRect());

    //draw all npcs on the screen
    for (int i = 0; i < _areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfNPC(); i++)
        SDL_BlitSurface(_areaData[_player->MapXLocation][_player->MapYLocation]->NpcData[i]->GetSprite(), 0, _screen, _areaData[_player->MapXLocation][_player->MapYLocation]->NpcData[i]->GetDestinationRect());

    //draw debug sprites
    if (_testSprite->Visible)
        SDL_BlitSurface(_testSprite->GetSprite(), 0, _screen,  _testSprite->GetDestinationRect());


}

void Engine::RenderBackground()
{
    //blits tiles onto the screen. tile is selected by selecting clip section of tilesheet at location
    //set up during tile setup.
    for (int i = 0; i < NUM_TILES; i++){
        SDL_BlitSurface(_tileSheet->GetSprite(), _clips[_tiles[i]->Type], _screen, _tiles[i]->Box);
    }
}

void Engine::UpdateScreen()
{
    SDL_Flip(_screen);
}

/// Handles all running engine events
void Engine::HandleEvents()
{
        //check if we need a refresh
    if (_refreshRequired == true){
        setUpTiles();
        _refreshRequired = false;
    }

    //handles player projectile moving across screen
    if (_player->Weapon->Fired())
    {
        //verify sprite is in playfield boundries
        if ((_player->Weapon->X <= PLAYFIELD_WIDTH && _player->Weapon->X >= 1)
                && (_player->Weapon->Y <= PLAYFIELD_HEIGHT && _player->Weapon->Y >= 1)) {

            //move projectile. (It will keep going in whatever direction it was fired.)
            _player->Weapon->Move();
        }
        else
            _player->Weapon->Visible = false;
    }

    //collision detection
    bool playerCollision = false;
    bool badGuyCollision = false;

    //bad guy collision detection
    for (int i = 0; i < _areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfBadGuys(); i++){
        if (_areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->Alive()){

            _areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->Move(_player->X, _player->Y);

            //check collision between badguy and player
            if (!badGuyCollision && checkCollision(_areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->GetDestinationRect(), _player->GetDestinationRect())){
                _player->Damage(_areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->GetDamage());
                playerCollision = true;
                badGuyCollision = true;
            }

            //check collision between projectile and badguy
            if (_player->Weapon->Visible){
                if (!badGuyCollision && checkCollision(_areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->GetDestinationRect(), _player->Weapon->GetDestinationRect())){

                    //damange badguy and hide projectile object
                    _player->Weapon->Visible = false;
                    _areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->Health -= _player->Weapon->GetDamage();
                    badGuyCollision = true;
                }
            }

            //check badguy and npc
            if (!badGuyCollision){
                for (int x = 0; x < _areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfNPC(); x++){
                    if (checkCollision(_areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->GetDestinationRect(), _areaData[_player->MapXLocation][_player->MapYLocation]->NpcData[x]->GetDestinationRect())){
                        badGuyCollision = true;
                    }
                }
            }

            //check bad guy and bad buy
            if (!badGuyCollision){
                for (int x = 0; x < _areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfBadGuys(); x++){
                    if (i != x && _areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[x]->Alive()){
                        if (checkCollision(_areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->GetDestinationRect(), _areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[x]->GetDestinationRect())){
                            badGuyCollision = true;
                        }
                    }
                }
            }

            //check bad guys on boundry tiles.
            if (!badGuyCollision){
                for (int x = 0; x < NUM_TILES; x++){
                    if (_tiles[x]->Type == 1 || _tiles[x]->Type >= 21){
                        if (checkCollision(_areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->GetDestinationRect(), _tiles[x]->Box)){
                            badGuyCollision = true;
                        }
                    }
                }
            }

            if (badGuyCollision)
                _areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->KnockBack();

        }
        badGuyCollision = false;
    }

    //npc collision detection as well as message setting
    for (int i = 0; i < _areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfNPC(); i++){
        if (checkCollision(_areaData[_player->MapXLocation][_player->MapYLocation]->NpcData[i]->GetDestinationRect(), _player->GetDestinationRect())){

            //set messenger text as well as set as visible
            _msgr[CONVERSATION]->SetSprite(TTF_RenderText_Solid(_font, _areaData[_player->MapXLocation][_player->MapYLocation]->NpcData[i]->GetText(), _msgr[CONVERSATION]->TextColor), 10, PLAYFIELD_HEIGHT + 75);
            _msgr[CONVERSATION]->Visible = true;
            playerCollision = true;
        }
    }

    //item collision detection
    for (int i = 0; i < _areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfItems(); i++){
        InventoryItem *tempItem = _areaData[_player->MapXLocation][_player->MapYLocation]->InventoryData->GetItem(i);
        if (NULL != tempItem){
            if (tempItem->Visible){
                if (checkCollision(_player->GetDestinationRect(), tempItem->GetDestinationRect())){
                    //add item to players inventory if possible.
                    if (_player->InventoryData->AddItem(new InventoryItem(*tempItem))){
                        //remove item from playfield if player picked up.
                        _areaData[_player->MapXLocation][_player->MapYLocation]->InventoryData->RemoveItem(i);
                    } else {
                        printf("Failed to add item. Inventory full.\n");
                    }
                }
            }
        }
    }

    //check player collision with boundry.
    for (int i = 0; i < NUM_TILES; i++){
        if (_tiles[i]->Type == 1 || _tiles[i]->Type >= 21){
            if (checkCollision(_player->GetDestinationRect(), _tiles[i]->Box)){
               playerCollision = true;
            }
        }
    }

    //if there was a collison, knock player back
    if (playerCollision){
        _player->KnockBack();
    } else {
        if (_player->Hurt) _player->ClearDamage();
    }

}

///Starts the firing process of players' projectile.
void Engine::FireProjectile()
{
    if (_player->Alive())
        _player->Weapon->Fire(_player->X, _player->Y, _player->Direction);
}

///Moves player
void Engine::MovePlayer(Directions direction)
{
    //clear conversations when player moves.
    if (_msgr[CONVERSATION]->Visible) _msgr[CONVERSATION]->Visible = false;

    if (_player->Alive()){

        if (_player->X >= PLAYFIELD_WIDTH - _player->GetStepDistance()) {
            _refreshRequired = true;
            _player->MoveToScreen(RIGHT);
        }
        else if (_player->X <= _player->GetStepDistance()) {
            _refreshRequired = true;
            _player->MoveToScreen(LEFT);
        }
        else if (_player->Y >= PLAYFIELD_HEIGHT - _player->GetStepDistance()) {
            _refreshRequired = true;
            _player->MoveToScreen(DOWN);
        }
        else if (_player->Y <= _player->GetStepDistance()) {
            _refreshRequired = true;
            _player->MoveToScreen(UP);
        }
        else
            _player->Move(direction);
    }
}

/// attempt to use item from inventory.
void Engine::UseItem(ItemType type){

    if (_player->Alive()){

        bool inventorySearchFinished = false;
        int i = 0;

        //cycle through inventory until item is found to use. After used, stop search.
        while(!inventorySearchFinished && i < _player->InventoryData->GetNumSlots()){
            InventoryItem *item = _player->InventoryData->GetItem(i);
            if (type == item->Type){

                switch (item->Type){
                    //equip weapon if found.
                    case WEAPON:
                        _player->Weapon->SetProjectile(item->Damage, item->Distance, item->StepDistance);
                        inventorySearchFinished = true;
                        break;

                    //use health item if found.
                    case HEALTH_ITEM:
                        if (_player->Heal(item->HealthValue)){
                            _player->InventoryData->RemoveItem(i);
                            inventorySearchFinished = true;
                        }
                        break;

                    //use key if found. need to add "bool" for when usable.
                    case KEY:
                        inventorySearchFinished = true;
                        break;
                }
            }
            i++;
        }
    }

}

/// Debug test methods
void Engine::ResetBadGuy()
{
    for (int i = 0; i < _areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfBadGuys(); i++)
    {
        _areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->Visible = true;
        _areaData[_player->MapXLocation][_player->MapYLocation]->BadGuyData[i]->Health = 100;
    }
}

void Engine::Restart()
{
    _player->Health = 100;
    initializeSprites();
    setUpTiles();
}

void Engine::PrintPlayerInventory(){
    printf("\n\nAREA INVENTORY\n-------------------\n");
    for (int i = 0; i < _areaData[_player->MapXLocation][_player->MapYLocation]->InventoryData->GetNumSlots(); i++){
        InventoryItem *item = _areaData[_player->MapXLocation][_player->MapYLocation]->InventoryData->GetItem(i);
        printf("itemname: %s\nitemtype %d\nitemdamage: %d\n", item->Name.c_str(), item->Type, item->Damage);
    }

    printf("\n\n\nINVENTORY:\n--------------------\n");
    for (int i = 0; i < _player->InventoryData->GetNumSlots(); i++){

        InventoryItem *item = _player->InventoryData->GetItem(i);
        printf("itemname: %s\nitemtype %d\nitemdamage: %d\n", item->Name.c_str(), item->Type, item->Damage);
    }
}

// Private methods

/// initializes Sprites and player/badguy objects
void Engine::initializeSprites()
{
    //printf("setting up sprites...");
    //printf("\nnumbadguys: %d\nnumnpc: %d\n",_areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfBadGuys(),_areaData[_player->MapXLocation][_player->MapYLocation]->GetNumberOfNPC());

    _tileSheet = new Sprite(TILE_FILENAME.c_str(), 1, 1, true);

    _testSprite = new Sprite(TEST_FILENAME.c_str(), 1, 1, false);

    _player = new Player(PLAYERSPRITE_FILENAME.c_str(), 655, 55, true);

    //set up messenger sprites.
    for (int i = 0; i < NUM_MESSENGERS; i++)
    {
        _msgr[i] = new Messenger();
    }

    _msgr[HEALTH]->SetSprite(TTF_RenderText_Solid(_font, _player->HealthString(), _msgr[HEALTH]->TextColor), 10, PLAYFIELD_HEIGHT + 45);
    _msgr[HEALTH]->Visible = true;

    printf("done!\n");

}

void Engine::setUpAreaData() {

    _refreshRequired = false;

    for (int x = 0; x < NUM_X_SCREENS; x++){
        for (int y = 0; y < NUM_Y_SCREENS; y++){
            printf("setting up areadata %d,%d\n", x, y);

            //filenames are level + x/y map locations.
            std::ostringstream strStream;
            strStream << "level" << x << y << ".map";

            //get number of npc, bad guys, items for the area.
            std::ostringstream dataFilename;
            dataFilename << "level" << x << y << ".dat";

            std::ifstream dataFile(dataFilename.str().c_str());
            int tempBadGuyNum = 0, tempNpcNum = 0, tempItemNum = 0;

            if (NULL != dataFile){

                dataFile >> tempBadGuyNum;
                dataFile >> tempNpcNum;
                dataFile >> tempItemNum;

                //check for failure.
                if (dataFile.fail() == true) {
                    printf("setAreaData: Failed to read data file.\n");
                    dataFile.close();
                }

                dataFile.close();
            }

            //badguy, npc, item numbers / info comes from levelxy.dat files.
            _areaData[x][y] = new AreaData(tempBadGuyNum, tempNpcNum, tempItemNum, strStream.str());


            if (x == 0 && y == 0){
                _areaData[x][y]->InventoryData->AddItem(new InventoryItem("PISTOL", WEAPON, 275, 75, true));
                _areaData[x][y]->InventoryData->AddItem(new InventoryItem("GRAVEYARD KEY", KEY, 275, 180, true));
                _areaData[x][y]->InventoryData->AddItem(new InventoryItem("MEDKIT", HEALTH_ITEM, 475, 280, true));;
                _areaData[x][y]->InventoryData->AddItem(new InventoryItem("MEDKIT", HEALTH_ITEM, 475, 180, true));;
                _areaData[x][y]->InventoryData->AddItem(new InventoryItem("MEDKIT", HEALTH_ITEM, 475, 230, true));;
                _areaData[x][y]->InventoryData->AddItem(new InventoryItem("MEDKIT", HEALTH_ITEM, 425, 380, true));;
                _areaData[x][y]->InventoryData->AddItem(new InventoryItem("MEDKIT", HEALTH_ITEM, 625, 280, true));;
                _areaData[x][y]->InventoryData->AddItem(new InventoryItem("MEDKIT", HEALTH_ITEM, 625, 180, true));;

            }

            //allocate bad guy and npc array items memory.
            for (int i = 0; i < tempBadGuyNum; i++)
                _areaData[x][y]->BadGuyData[i] = new BadGuy(BADGUY_FILENAME.c_str(), (rand() % PLAYFIELD_WIDTH), (rand() % PLAYFIELD_HEIGHT), true);

            //get npc data for the area.
            for (int i = 0; i < tempNpcNum; i++){
                std::ostringstream npcDataFilename;
                npcDataFilename << "level" << x << y << ".npc";

                std::ifstream npcDataFile(npcDataFilename.str().c_str());

                int tempX = 0, tempY = 0;
                std::string tempText = "";

                if (NULL != npcDataFile){
                    npcDataFile >> tempX;
                    npcDataFile >> tempY;
                    npcDataFile >> tempText;

                    //check for failure.
                    if (npcDataFile.fail() == true) {
                        printf("setAreaData: Failed to read npc data file.\n");
                        npcDataFile.close();
                    }
                    npcDataFile.close();
                }


                _areaData[x][y]->NpcData[i] = new NPC(NPC_FILENAME.c_str(), tempX, tempY, true, tempText.c_str());
            }

        }
    }
}

bool Engine::setUpTiles(){
    printf("Setting up tiles...\n");
    int x = 0; int y = 0;

    int tileType = 0;

    printf("Setting tiles from file: %s\n", _areaData[_player->MapXLocation][_player->MapYLocation]->GetMapFileName().c_str());
    std::ifstream map(_areaData[_player->MapXLocation][_player->MapYLocation]->GetMapFileName().c_str());

    if (NULL == map) return false;

    for (int t = 0; t < NUM_TILES; t++){
        map >> tileType;

        //check for failure.
        if (map.fail() == true) {
            printf("setUpTiles: Failed to read map file.\n");
            map.close();
            return false;
        }

        //add tile if valid.
        if (tileType >= 0 && tileType <= TOTAL_TILE_SPRITES){
            _tiles[t] = new Tile(x, y, tileType);
        }

        //if we've gone to the end of the screen, move to the next row.
        x += TILE_WIDTH;
        if (x >= PLAYFIELD_WIDTH){
            x = 0;
            y += TILE_HEIGHT;
        }
    }

    map.close();
    printf("Done!\n");
    return true;
}

void Engine::clipTiles(){

    printf("clipping tiles...");
    //dynamically creates drawing rect for each tile image
    //based on the size of image file width.
    int x = 0, y = 0;

    for (int i = 0; i < TOTAL_TILE_SPRITES; i++) {

        _clips[i] = new SDL_Rect();
        _clips[i]->x = x;
        _clips[i]->y = y;
        _clips[i]->h = TILE_HEIGHT;
        _clips[i]->w = TILE_WIDTH;

        x += TILE_WIDTH;

        if (x >= TILE_MAP_IMAGE_WIDTH){
            x = 0;
            y += TILE_HEIGHT;
        }
    }

    printf("done!\n");
}

bool Engine::checkCollision(SDL_Rect *rect1, SDL_Rect *rect2){

    bool collision = false;

    //check boundries first before checking anything else.
    if (rect1->x <= 0 || rect1->x >= PLAYFIELD_WIDTH) return true;
    if (rect1->y <= 0 || rect1->y >= PLAYFIELD_HEIGHT) return true;

    if (rect1->x <= (rect2->x + rect2->w) && (rect1->x + rect1->w) >= rect2->x
        && rect1->y <= (rect2->y + rect2->h) && (rect1->y + rect1->h) >= rect2->y )
    {
        collision = true;
    }

    collision ? _testSprite->Visible = true : _testSprite->Visible = false;

    return collision;
}
