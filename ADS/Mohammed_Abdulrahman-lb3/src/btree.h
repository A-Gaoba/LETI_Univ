#pragma once

namespace binTree_modul
{

//-------------------------------------
	typedef char base;
	
	struct node {
		base info;
		node *lt;
		node *rt;
		// constructor
		node ()
		{
			lt = NULL; rt = NULL;
		}
	};
	
	typedef node *binTree; // binary tree "representative"

	binTree Create(void);
	bool isNull(binTree);
	base RootBT (binTree); // for a non-empty bin.tree
	binTree Left (binTree);// for a non-empty bin.tree
	binTree Right (binTree);// for a non-empty bin.tree
	binTree ConsBT(const base x, binTree lst,  binTree rst);
	void destroy (binTree&);
	bool isEqual(binTree a,  binTree b);

} // end of namespace binTree_modul

/*
	1) RootBT: NonNullBT -> ?;
	2) Left: NonNullBT -> BT;
	3) Right: NonNullBT -> BT;
	4) ConsBT: ? * BT * BT -> NonNullBT;
	5) Null: BT -> Boolean;
	6) Create:  -> BT 
*/