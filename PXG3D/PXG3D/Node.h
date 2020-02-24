#pragma once
namespace PXG
{
	class Node
	{
	public:
		Node();
		~Node();
		static int GetNodeWheight();
		void SetNodeWeight(int newWeight);
	private:
		int nodeWeight;
	};


}