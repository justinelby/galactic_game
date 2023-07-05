/*
    //Testing first character
    if (!characterMap.empty()) {
        string characterName = "Alex Starborn";
        auto it = characterMap.find(characterName);
        if (it != characterMap.end()) {
            auto character = it->second;
#ifdef DEBUG
            cout << "Nom : " << character->getName() << endl;
            cout << "Sante : " << character->getHealth() << endl;
#endif
            character->setHealth(22);

            cout << "Sante modifiee : " << character->getHealth() << endl;
        } else {
            cout << "Personnage non trouvé : " << characterName << endl;
        }
    } else {
        cout << "Il n'y a pas de personnage disponible." << endl;
    }


    //Testing first spaceship
    cout << "\n------Testing first spaceship crew------" << endl;
    cout << "Nom crew1 : " << controller.getSpaceship()[0]->getCrew()[0].lock()->getName() << endl;
    cout << "Sante crew1 : "  << controller.getSpaceship()[0]->getCrew()[0].lock()->getHealth() << endl;

    //Testing first planet
    cout << "\n------Testing first planet residents------" << endl;
    cout << "Nom resident1 : " << controller.getPlanet()[1]->getResident()[0].lock()->getName() << endl;
    cout << "Sante resident1 : " << controller.getPlanet()[1]->getResident()[0].lock()->getHealth() << endl;

    // Testing character deletion
    cout << "\n------Testing character deletion------" << endl;
    cout << "---Equipage de USS Enterprise crew before character deletion :" << endl;

    controller.getSpaceship()[0]->showCrew();*/



/*    cout << "---Equipage de USS Enterprise crew after character deletion :" << endl;
    controller.getSpaceship()[0]->showCrew();

    // Testing spaceship deletion
    cout << "\n------Testing spaceship deletion------" << endl;
    cout << "---Character list before spaceship deletion :" << endl;
    for(auto& pair : characterMap){
        auto c = pair.second;
        cout << c->getName() <<endl;
    }
    cout << "---Initial list of spaceships:" << endl;
    for(auto& s : controller.getSpaceship()){
        cout << s->getName() <<endl;
    }
    controller.deleteSpaceship(0);//on supprime USS Enterprise
    cout << "---New list of spaceships:" << endl;
    for(auto& s : controller.getSpaceship()){
        cout << s->getName() <<endl;
    }
    cout << "---Character list after spaceship deletion :" << endl;
    for(auto& pair : characterMap){
        auto c = pair.second;
        cout << c->getName() <<endl;
    }*/