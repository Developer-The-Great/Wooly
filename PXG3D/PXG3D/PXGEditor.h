#pragma once
#include "Game.h"
#include "font.h"
#include "TextureMaterial.h"

namespace PXG {
    class PXGEditor : public Game
    {
    public:

	    void SetupCamera();
        void Start() override;

	    void CreateEditorBlock(const std::string& model, const std::string& texture);
        void SaveMap();


        void Update() override;

        void FixedUpdate(float tick) override;

        void Initialize() override;

        GameObj MakeChildObj(const std::string& name);
	    GameObj MakeCanvasObj(const std::string& name);




	    Font* font;
        FontRenderer* frender;

        GameObj map;
        GameObj Player;


    };
}

