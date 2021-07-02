//
// Created by Nguyen Dinh Luong on 13/5/21.
//

#ifndef ASSIGNMENT2_DSA__AVL_ADT_H
#define ASSIGNMENT2_DSA__AVL_ADT_H
//	==================== MACROS ====================
#define LH +1     // Left High
#define EH  0     // Even High
#define RH -1     // Right High

// NODE Definitions
template <class TYPE>
struct NODE {
    TYPE   data;
    NODE   *left;
    NODE   *right;
    int     bal;
}; // NODE

// Class Declaration
template <class TYPE, class KTYPE>
class AvlTree
{
private:
    int count;
    int countWithDuplication;
    int countIndex;
    int countPhrases;
    int updateCountPhrase;
    NODE<TYPE>  *tree;
    NODE<TYPE> *_insert (NODE<TYPE>  *root, NODE<TYPE> *newPtr, bool &taller);
    NODE<TYPE> *insert (NODE<TYPE>  *root, NODE<TYPE> *newPtr, bool &taller);
    void printInOrder(NODE<TYPE>* root,ofstream& file);
    void printBFS(NODE<TYPE>* root,ofstream& file);
    void frequencyPerThousandWords(NODE<TYPE>* root);
    NODE<TYPE>  *leftBalance (NODE<TYPE>  *root, bool &taller);
    NODE<TYPE>  *rotateLeft (NODE<TYPE>  *root);
    NODE<TYPE>  *rightBalance (NODE<TYPE>  *root,bool &taller);
    NODE<TYPE>  *rotateRight (NODE<TYPE>  *root);
    NODE<TYPE> *_delete (NODE<TYPE>  *root,KTYPE dltKey,bool &shorter,bool &success);
    NODE<TYPE>  *dltLeftBalance (NODE<TYPE>  *root,bool &smaller);
    NODE<TYPE>  *dltRightBalance (NODE<TYPE>  *root,bool &shorter);
    NODE<TYPE> *_retrieve (KTYPE key,  NODE<TYPE>  *root);
    NODE<TYPE> *retrieve (KTYPE key,  NODE<TYPE>  *root);

    void  _traversal (void (*process)(TYPE& dataProc), NODE<TYPE> *root);

    void  traversal (vector<KTYPE> words, vector<KTYPE>& vector, NODE<TYPE> *root);

    void  traversalThreshold (double threshold,map<string,double> newMap,NODE<TYPE> *root);

    void  traversalFindCharacter (vector<char> charVector,priority_queue<TYPE>& priorityQueue,NODE<TYPE> *root);

    void  _destroyAVL (NODE<TYPE>  *root);

//  	The following function is used for debugging.
    void  _print (NODE<TYPE> *root, int  level);
    void   print (NODE<TYPE> *root, int  level);

public:
    AvlTree (void);
    ~AvlTree  (void);
    bool  AVL_Insert       (TYPE dataIn);
    bool  AVL_InsertWord   (TYPE dataIn);
    bool  AVL_Delete       (KTYPE  dltKey);
    bool  AVL_Retrieve (KTYPE key,TYPE &dataOut);
    bool  AVL_RetrieveWord (KTYPE  key,TYPE &dataOut);
    void  printNodeInOrder(string path);
    void  printNodeBFS(string path);
    void  updateFrequencyPerThousandWords();
    void  AVL_Traverse (void (*process)(TYPE& dataProc)); //in-order
    void  AVL_TraverseThreshold (double,map<string,double>); //in-order
    void  AVL_TraverseUpdate (vector<KTYPE>,vector<KTYPE>&); //in-order
    void  AVL_TraverseFindCharacter (vector<char>,priority_queue<TYPE>&); //in-order
    void  update(TYPE& data);
    void  updateIndex(list<int>& list);
    int getCount();
    int getCountWithDuplication();
    int getCountPhrases();
    int getUpdateCountPhrases();
    bool stringIncludeVectorOfCharacters(vector<char>,string);
    bool  AVL_Empty    (void);
    bool  AVL_Full     (void);
    int   AVL_Count    (void);

    // The following function is used for debugging.
    void  AVL_Print    (void);
    void  AVL_PrintWord    (void);
};  // class AvlTree

/*	=================== Constructor ===================
	Initializes private data.
	   Pre     class is being defined
	   Post    private data initialized
*/

template <class TYPE, class KTYPE>
AvlTree<TYPE, KTYPE> :: AvlTree (void)
{
//Statements
    tree    = NULL;
    count   = 0;
    countWithDuplication = 0;
    countIndex = 0;
    countPhrases = 0;
    updateCountPhrase = 0;
}	//Constructor

/*	==================== AVL_Insert ====================
	This function inserts new data into the tree.
	   Pre    dataIn contains data to be inserted
	   Post   data have been inserted or memory overflow
	   Return success (true) or overflow (false)
*/

template <class TYPE, class KTYPE>
bool AvlTree<TYPE, KTYPE> :: AVL_Insert (TYPE dataIn)
{
//	Local Definitions
    NODE<TYPE>  *newPtr;
    bool taller;
//	Statements
    if (!(newPtr = new NODE<TYPE>))
        return false;
    newPtr->bal    = EH;
    newPtr->right  = NULL;
    newPtr->left   = NULL;
    newPtr->data   = dataIn;

    tree = _insert(tree, newPtr, taller);
    count++;
    countIndex++;
    countWithDuplication++;
    return true;
}	//  Avl_Insert

template <class TYPE, class KTYPE>
bool AvlTree<TYPE, KTYPE> :: AVL_InsertWord(TYPE dataIn)
{
//	Local Definitions
    NODE<TYPE>  *newPtr;
    bool taller;
//	Statements
    if (!(newPtr = new NODE<TYPE>))
        return false;
    newPtr->bal    = EH;
    newPtr->right  = NULL;
    newPtr->left   = NULL;
    newPtr->data   = dataIn;

    tree = insert(tree, newPtr, taller);
    count++;
    return true;
}	//  Avl_Insert

/*	======================= _insert =======================
	This function uses recursion to insert the new data into
	a leaf node in the AVL tree.
	   Pre    application has called AVL_Insert, which passes
	          root and data pointers
	   Post   data have been inserted
	   Return pointer to [potentially] new root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE,  KTYPE> :: _insert (NODE<TYPE> *root, NODE<TYPE> *newPtr,bool &taller)
{
    //Statements
    if (!root)
    {
        root    = newPtr;
        taller  = true;
        updateIndex(root->data.positions);
        return  root;
    } //  if NULL tree
    if (newPtr->data.key < root->data.key)
    {
        root->left = _insert(root->left,newPtr,taller);
        if (taller)
            //  Left subtree is taller
            switch (root->bal)
            {
                case LH: // Was left high--rotate
                    root = leftBalance (root, taller);
                    break;

                case EH: // Was balanced--now LH
                    root->bal = LH;
                    break;

                case RH: // Was right high--now EH
                    root->bal = EH;
                    taller    = false;
                    break;
            } // switch
    } // new < node
    else if(newPtr->data.key > root->data.key)
        //  new data > root data
    {
        root->right = _insert (root->right,newPtr,taller);
        if (taller)
            //Right subtree is taller
            switch (root->bal)
            {
                case LH: // Was left high--now EH
                    root->bal = EH;
                    taller    = false;
                    break;
                case EH: // Was balanced--now RH
                    root->bal = RH;
                    break;
                case RH: // Was right high--rotate
                    root = rightBalance (root, taller);
                    break;
            } //switch
    } // else new data > root data
    else {
        update(root->data);
        updateIndex(root->data.positions);
        count--;
    } // else new data = root data
    return root;
}	//_insert

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE,  KTYPE> :: insert (NODE<TYPE> *root, NODE<TYPE> *newPtr,bool &taller)
{
    //Statements
    if (!root)
    {
        root    = newPtr;
        taller  = true;
        return  root;
    } //  if NULL tree
    if (newPtr->data.uniquePosition < root->data.uniquePosition)
    {
        root->left = insert(root->left,newPtr,taller);
        if (taller)
            //  Left subtree is taller
            switch (root->bal)
            {
                case LH: // Was left high--rotate
                    root = leftBalance (root, taller);
                    break;

                case EH: // Was balanced--now LH
                    root->bal = LH;
                    break;

                case RH: // Was right high--now EH
                    root->bal = EH;
                    taller    = false;
                    break;
            } // switch
    } // new < node
    else {
        //  new data >= root data
        root->right = insert (root->right,newPtr,taller);
        if (taller)
            //Right subtree is taller
            switch (root->bal)
            {
                case LH: // Was left high--now EH
                    root->bal = EH;
                    taller    = false;
                    break;
                case EH: // Was balanced--now RH
                    root->bal = RH;
                    break;
                case RH: // Was right high--rotate
                    root = rightBalance (root, taller);
                    break;
            } //switch
    }
    // else new data >= root data
    return root;
}	//insert

/*	===================== leftBalance =====================
	The tree is out of balance to the left. This function
	rotates the tree to the right.
	   Pre     the tree is left high
	   Post    balance restored
	   Returns potentially new root
*/
template <class TYPE, class KTYPE>
void AvlTree<TYPE,  KTYPE> ::printInOrder(NODE<TYPE>* root,ofstream& file)
{
    if(root != NULL)
    {
        printInOrder(root->left,file);
        file<<(root->data.key) <<" "<<(root->data.frequency);
        list<int>::iterator it;
        for (it = root->data.positions.begin(); it != root->data.positions.end(); it++)
        {
            file<<" ("<<root->data.path<<","<<(*it)<<")"<<" ";
        }
        file<<"\n";
        printInOrder(root->right,file);
    }
}

template <class TYPE, class KTYPE>
void AvlTree<TYPE,  KTYPE> :: frequencyPerThousandWords(NODE<TYPE>* root) {
    if(root != NULL)
    {
        frequencyPerThousandWords(root->left);
        root->data.frequencyPerThousandWords = root->data.frequencyPerThousandWords/getCountWithDuplication()*1000;
        frequencyPerThousandWords(root->right);
    }
}

template <class TYPE, class KTYPE>
void AvlTree<TYPE,  KTYPE> :: updateFrequencyPerThousandWords() {
    frequencyPerThousandWords(tree);
}

template <class TYPE, class KTYPE>
void AvlTree<TYPE,  KTYPE>::printNodeInOrder(string path)
{
    ofstream file;
    file.open (path.c_str());
    printInOrder(tree,file);
    file.close();
}

template <class TYPE, class KTYPE>
void AvlTree<TYPE,  KTYPE>::printBFS(NODE<TYPE> *root,ofstream& file) {
    queue<NODE<TYPE>*> q;
    q.push(root);
    while(!q.empty()) {
        NODE<TYPE>* fnt = q.front();
        file<<(fnt->data.key) <<" "<<(fnt->data.frequency);
        list<int>::iterator it;


        for (it = fnt->data.positions.begin(); it != fnt->data.positions.end(); it++)
        {
            file<<" ("<<fnt->data.path<<","<<(*it)<<")"<<" ";
        }

        file<<"\n";

        q.pop();

        if(fnt->left) {
            q.push(fnt->left);
        }

        if (fnt->right) {
            q.push(fnt->right);
        }
    }
}

template <class TYPE, class KTYPE>
void AvlTree<TYPE,  KTYPE>:: printNodeBFS(string path) {
    ofstream file;
    file.open (path.c_str());
    printBFS(tree,file);
    file.close();
}

template <class TYPE, class KTYPE>
NODE<TYPE>  *AvlTree<TYPE,  KTYPE> :: leftBalance (NODE<TYPE>  *root,bool &taller)
{
// 	Local Definitions
    NODE<TYPE>  *rightTree;
    NODE<TYPE>  *leftTree;

//	Statements
    leftTree = root->left;
    switch (leftTree->bal)
    {
        case LH: // Left High--Rotate Right
            root->bal     = EH;
            leftTree->bal = EH;

            // Rotate Right
            root     = rotateRight (root);
            taller   = false;
            break;
        case EH: // This is an error
            cout <<"\n\a\aError in leftBalance\n";
            exit (100);
        case RH: // Right High - Requires double rotation:
            // first left, then right
            rightTree = leftTree->right;
            switch (rightTree->bal)
            {
                case LH: root->bal     = RH;
                    leftTree->bal = EH;
                    break;
                case EH: root->bal     = EH;
                    leftTree->bal = EH;
                    break;
                case RH: root->bal     = EH;
                    leftTree->bal = LH;
                    break;
            } //  switch rightTree

            rightTree->bal = EH;
            //  Rotate Left
            root->left = rotateLeft (leftTree);

            // Rotate Right
            root    = rotateRight (root);
            taller  = false;
    } // switch leftTree
    return root;
}	// leftBalance

/*	===================== rotateLeft ======================
	This function exchanges pointers so as to rotate the
	tree to the left.
	   Pre  root points to tree to be rotated
	   Post NODE rotated and new root returned
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE,  KTYPE> :: rotateLeft (NODE<TYPE>  *root)
{
//	Local Definitions
    NODE<TYPE>  *tempPtr;

//	Statements
    tempPtr        = root->right;
    root->right    = tempPtr->left;
    tempPtr->left  = root;

    return tempPtr;
}	//  rotateLeft

/*	===================== rotateRight =====================
	This function exchanges pointers to rotate the tree
	to the right.
	   Pre   root points to tree to be rotated
	   Post  NODE rotated and new root returned
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE,  KTYPE> :: rotateRight (NODE<TYPE> *root)
{
//	Local Definitions
    NODE<TYPE>  *tempPtr;

//	Statements
    tempPtr         = root->left;
    root->left      = tempPtr->right;
    tempPtr->right  = root;

    return tempPtr;
}	//  rotateRight

/*	====================  rightBalance ===================
	The tree is out-of-balance to the right. This function
	rotates the tree to the left.
	   Pre     The tree is right high
	   Post    Balance restored
	   Returns potentially new root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>* AvlTree<TYPE, KTYPE> :: rightBalance (NODE<TYPE> *root, bool& taller)
{

//	Local Definitions
    NODE<TYPE> *rightTree;
    NODE<TYPE> *leftTree;

//	Statements
    rightTree = root->right;
    switch (rightTree->bal)
    {
        case LH: // Left High - Requires double rotation:
            //             first right, then left
            leftTree = rightTree-> left;

            //  First Rotation - Left
            switch (leftTree->bal)
            {
                case RH: root->bal      = LH;
                    rightTree->bal = EH;
                    break;

                case EH: root->bal      = EH;
                    rightTree->bal = EH;
                    break;

                case LH: root->bal      = EH;
                    rightTree->bal = RH;
                    break;
            } //  switch
            leftTree->bal = EH;

            root->right   = rotateRight (rightTree);
            root          = rotateLeft  (root);
            taller        = false;
            break;

        case EH: // Deleting from balanced node
            root->bal = EH;
            taller    = false;
            break;

        case RH: // Right High - Rotate Left
            root->bal       = EH;
            rightTree->bal  = EH;
            root            = rotateLeft (root);
            taller          = false;
            break;
    } // switch
    return root;
}	//  rightBalance

/*	====================== AVL_Delete ======================
	This function deletes a node from the tree and rebalances
	it if necessary.
	   Pre    dltKey contains key to be deleted
	   Post   the node is deleted and its space recycled
	          -or- an error code is returned
	   Return success (true) or not found (false)
*/

template <class TYPE, class KTYPE>
bool  AvlTree <TYPE, KTYPE> :: AVL_Delete (KTYPE  dltKey)
{
//	Local Definitions
    bool shorter;
    bool success;

    NODE<TYPE>  *newRoot;

//	Statements
    newRoot = _delete (tree, dltKey, shorter, success);
    if (success)
    {
        tree = newRoot;
        count--;
    } // if
    return success;
}	// AVL_Delete

/*	======================== _delete =======================
	This function deletes a node from the tree and rebalances
	it if necessary.
	   Pre    dltKey contains key of node to be deleted
	          shorter is Boolean indicating tree is shorter
	   Post   the node is deleted and its space recycled
	          -or- if key not found, tree is unchanged
	   Return true if deleted, false if not found
	          pointer to root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE,  KTYPE>:: _delete (NODE<TYPE> *root,KTYPE dltKey,bool &shorter, bool &success)
{
//  Local Definitions
    NODE<TYPE> *dltPtr;
    NODE<TYPE> *exchPtr;
    NODE<TYPE> *newRoot;

// 	Statements
    if (!root)
    {
        shorter = false;
        success = false;
        return NULL;
    } //  if -- base case

    if (dltKey < root->data.key)
    {
        root->left = _delete (root->left, dltKey, shorter,success);
        if (shorter)
            root   = dltRightBalance (root, shorter);
    } // if less
    else if (dltKey > root->data.key)
    {
        root->right = _delete (root->right, dltKey,shorter,success);
        if (shorter)
            root = dltLeftBalance (root, shorter);
    } //  if greater
    else
        //  Found equal node
    {
        dltPtr  = root;
        if (!root->right)
            // Only left subtree
        {
            newRoot  = root->left;
            success  = true;
            shorter  = true;
            delete (dltPtr);
            return newRoot;            //  base case
        } //  if true
        else
        if (!root->left)
            //  Only right subtree
        {
            newRoot  = root->right;
            success  = true;
            shorter  = true;
            delete (dltPtr);
            return newRoot;        // base case
        } //  if
        else
            // Delete NODE has two subtrees
        {
            exchPtr = root->left;
            while (exchPtr->right)
                exchPtr = exchPtr->right;

            root->data = exchPtr->data;
            root->left = _delete (root->left, exchPtr->data.key,shorter,success);
            if (shorter)
                root = dltRightBalance (root, shorter);
        } //  else

    } // equal node
    return root;
}	// _delete

// ================== dltLeftBalance ==================
/*	The tree is out-of-balance to the left (left high)--
	rotates the tree to the right.
	   Pre     The tree is left high
	   Post    Balance has been restored
	   Returns potentially new root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE,  KTYPE> :: dltLeftBalance  (NODE<TYPE>  *root, bool &smaller)
{
//	Local Definitions
    NODE<TYPE>  *rightTree;
    NODE<TYPE>  *leftTree;

//	Statements
    switch (root->bal)
    {
        case RH:  root->bal	= EH;
            break;

        case EH:  // Delete occurred on right
            root->bal  = LH;
            smaller    = false;
            break;

        case LH:  leftTree = root->left;
            switch (leftTree->bal)
            {
                case LH:
                case EH: // Rotate Single Left
                    if (leftTree->bal  == EH)
                    {
                        root->bal     = LH;
                        leftTree->bal = RH;
                        smaller       = false;
                    } // if
                    else
                    {
                        root->bal     = EH;
                        leftTree->bal = EH;
                    } // else

                    root = rotateRight (root);
                    break;

                case RH:	//Double Rotation
                    rightTree = leftTree->right;
                    switch (rightTree->bal)
                    {
                        case LH: root->bal     = RH;
                            leftTree->bal = EH;
                            break;
                        case EH: root->bal     = EH;
                            leftTree->bal = EH;
                            break;
                        case RH: root->bal     = EH;
                            leftTree->bal = LH;
                            break;
                    } //  switch
                    rightTree->bal = EH;
                    root->left     = rotateLeft (leftTree);
                    root           = rotateRight (root);
                    break;
            } //  switch : leftTree->bal

    } //  switch : root->bal
    return root;
}	// dltLeftBalance

/*	=================== dltRightBalance ==================
	The tree is shorter after a delete on the left.
	Adjust the balance factors and rotate the tree
	to the left if necessary.
	   Pre     the tree is shorter
	   Post    balance factors adjusted and balance restored
	   Returns potentially new root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE,  KTYPE> ::  dltRightBalance (NODE<TYPE>  *root, bool &shorter)
{
//  Local Definitions
    NODE<TYPE>  *rightTree;
    NODE<TYPE>  *leftTree;

//	Statements
    switch (root->bal)
    {
        case LH: //  Deleted Left--Now balanced
            root->bal  = EH;
            break;
        case EH: //  Now Right high
            root->bal  = RH;
            shorter    = false;
            break;
        case RH: //  Right High - Rotate Left
            rightTree = root->right;
            if (rightTree->bal == LH)
                // Double rotation required
            {
                leftTree  = rightTree->left;

                switch (leftTree->bal)
                {
                    case LH: rightTree->bal = RH;
                        root->bal      = EH;
                        break;
                    case EH: root->bal      = EH;
                        rightTree->bal = EH;
                        break;
                    case RH: root->bal      = LH;
                        rightTree->bal = EH;
                        break;
                } // switch

                leftTree->bal = EH;

                // Rotate Right then Left
                root->right = rotateRight (rightTree);
                root        = rotateLeft  (root);
            } //  if rightTree->bal == LH
            else
            {
                //  Single Rotation Only
                switch (rightTree->bal)
                {
                    case LH:
                    case RH: root->bal      = EH;
                        rightTree->bal = EH;
                        break;
                    case EH: root->bal      = RH;
                        rightTree->bal = LH;
                        shorter        = false;
                        break;
                } // switch rightTree->bal
                root = rotateLeft (root);
            } // else
    } //  switch root bal
    return root;
}	//  dltRightBalance

/*	==================== AVL_Retrieve ===================
	Retrieve node searches the tree for the node containing
	the requested key and returns pointer to its data.
	   Pre     dataOut is variable to receive data
	   Post    tree searched and data returned
	   Return  true if found, false if not found
*/

template <class TYPE, class KTYPE>
bool AvlTree<TYPE, KTYPE> ::  AVL_Retrieve (KTYPE key, TYPE &dataOut)
{
//	Local Definitions
    NODE<TYPE> *node;

//	Statements
    if (!tree)
        return false;

    node = _retrieve (key, tree);
    if (node)
    {
        dataOut = node->data;
        return true;
    } // if found
    else
        return false;
}	//  AVL_Retrieve

template <class TYPE, class KTYPE>
bool   AvlTree<TYPE, KTYPE> ::  AVL_RetrieveWord (KTYPE key, TYPE &dataOut)
{
//	Local Definitions
    NODE<TYPE> *node;

//	Statements
    if (!tree)
        return false;

    node = retrieve (key, tree);
    if (node)
    {
        dataOut = node->data;
        return true;
    } // if found
    else
        return false;
}	//  AVL_Retrieve

/*	===================== _retrieve =====================
	Retrieve searches tree for node containing requested
	key and returns its data to the calling function.
	   Pre     AVL_Retrieve called: passes key to be located
	   Post    tree searched and data pointer returned
	   Return  address of matching node returned
	           if not found, NULL returned
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE, KTYPE> ::  _retrieve (KTYPE key, NODE<TYPE> *root)
{
//	Statements
    if (root)
    {
        if (key < root->data.key)
            return _retrieve (key, root->left);
        else if (key > root->data.key)
            return _retrieve (key, root->right);
        else
            // Found equal key
            return (root);
    } // if root
    else
        //Data not in tree
        return root;
}	//  _retrieve

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE, KTYPE> ::  retrieve (KTYPE key, NODE<TYPE> *root)
{
//	Statements
    if (root)
    {
        if (key < root->data.uniquePosition)
            return retrieve (key, root->left);
        else if (key > root->data.uniquePosition)
            return retrieve (key, root->right);
        else
            // Found equal key
            return (root);
    } // if root
    else
        //Data not in tree
        return root;
}	//  _retrieve

/*	==================== AVL_Traverse ====================
	Process tree using inorder traversal.
	   Pre   process used to "visit" nodes during traversal
	   Post  all nodes processed in LNR (inorder) sequence
*/

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> :: AVL_Traverse (void (*process)(TYPE& dataProc))
{
//	Statements
    _traversal (process, tree);
    return;
}	// end AVL_Traverse

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> :: AVL_TraverseThreshold(double threshold,map<string,double> newMap) {
    //	Statements
    updateCountPhrase = 0;
    traversalThreshold(threshold,newMap,tree);
    return;
}
// end AVL_TraverseThreshold

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> :: AVL_TraverseUpdate(vector<KTYPE> words,vector<KTYPE>& vector)
{
//	Statements
    traversal (words,vector,tree);
    return;
}	// end AVL_Traverse

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> :: AVL_TraverseFindCharacter(vector<char> vectorChar, priority_queue<TYPE>& priorityQueue) {
    while (!priorityQueue.empty()) {
        priorityQueue.pop();
    }
    traversalFindCharacter(vectorChar,priorityQueue,tree);
    return;
}

/*	===================== _traversal =====================
	Traverse tree using inorder traversal. To process a
	node, we use the function passed when traversal is called.
	   Pre   tree has been created (may be null)
	   Post  all nodes processed
*/

template <class TYPE, class KTYPE>
void AvlTree<TYPE,KTYPE> :: update(TYPE &data) {
    data.frequency = data.frequency + 1;
    data.frequencyPerThousandWords = data.frequencyPerThousandWords + 1.0;
    data.frequencyPhrasePerThousandWords = data.frequencyPhrasePerThousandWords + 1.0;
}

template <class TYPE, class KTYPE>
void AvlTree<TYPE,KTYPE> :: updateIndex(list<int>& listIndex) {
    listIndex.push_back(countIndex);
}

template <class TYPE, class KTYPE>
int AvlTree<TYPE,KTYPE> :: getCount()  {
    return count;
}

template <class TYPE, class KTYPE>
int AvlTree<TYPE,KTYPE> :: getCountWithDuplication() {
    return countWithDuplication;
}

template <class TYPE, class KTYPE>
int AvlTree<TYPE,KTYPE> :: getCountPhrases() {
    return countPhrases;
}

template <class TYPE, class KTYPE>
int AvlTree<TYPE,KTYPE> :: getUpdateCountPhrases() {
    return updateCountPhrase;
}

template <class TYPE, class KTYPE>
bool AvlTree<TYPE,KTYPE> :: stringIncludeVectorOfCharacters(vector<char> vector, string str) {
    std::vector<char> vc(str.begin(),str.end());
    if(vc.size() < vector.size()) {
        return false;
    } else {
        for(int i=0;i<vector.size();i++) {
            if(vc.at(i) != vector.at(i)) {
                return false;
            }
        }
        return true;
    }
}


template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> ::  _traversal (void(*process)(TYPE& dataproc), NODE<TYPE> *root)
{
//	Statements
    if (root)
    {
        _traversal  (process, root->left);
        map<string,double>::iterator it;
        for (it = root->data.includeKey.begin(); it != root->data.includeKey.end(); it++)
        {
            (*it).second = (*it).second/getCountPhrases()*1000;
        }
        process     (root->data);
        _traversal  (process, root->right);
    } //  if
    return;
}	//  _traversal

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> ::  traversalThreshold(double threshold,map<string,double> newMap,NODE<TYPE> *root) {
    if (root)
    {
        traversalThreshold(threshold,newMap,root->left);
        map<string,double>::iterator it;
        for (it = root->data.includeKey.begin(); it != root->data.includeKey.end(); it++)
        {
            if(!isgreater(threshold,(*it).second)) {
                newMap.insert(pair<string ,double>((*it).first,(*it).second/1000*getCountPhrases()));
                updateCountPhrase++;
            }
        }
        root->data.includeKey = newMap;
        newMap.clear();
        traversalThreshold(threshold,newMap,root->right);
    } //  if
    return;
}	//  traversalThreshold

template <class TYPE, class KTYPE>
void AvlTree<TYPE,KTYPE> :: traversalFindCharacter(vector<char> charVector, priority_queue<TYPE> &priorityQueue, NODE<TYPE> *root)  {

    if(root) {
        traversalFindCharacter(charVector,priorityQueue,root->left);
        if(stringIncludeVectorOfCharacters(charVector,root->data.key)) {
            priorityQueue.push(root->data);
        }
        traversalFindCharacter(charVector,priorityQueue,root->right);
    }
}

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> ::traversal (vector<KTYPE> words, vector<KTYPE>& vector, NODE<TYPE> *root)
{
//	Statements
    if (root)
    {
        traversal(words,vector, root->left);
        countPhrases++;
        vector.push_back(root->data.key);
        root->data.includeKey.insert(pair<string ,double>(root->data.key,root->data.frequencyPhrasePerThousandWords));
        list<int>::iterator it;
        for (it = root->data.positions.begin(); it != root->data.positions.end(); it++)
        {
            if((*it) < getCountWithDuplication()-2) {
                KTYPE phrases = words[(*it)];
                if(phrases != root->data.key) {
                    if(find(vector.begin(), vector.end(), phrases) == vector.end()) {
                        countPhrases++;
                        vector.push_back(phrases);
                        root->data.includeKey.insert (pair<string ,double>(phrases,0.5));
                    }
                }
                for (int k = (*it)+1; k < (*it)+3; k++)
                {
                    phrases = phrases +" "+words[k];
                    if (find(vector.begin(), vector.end(), phrases) == vector.end()) {
                        countPhrases++;
                        vector.push_back(phrases);
                        root->data.includeKey.insert (pair<string ,double>(phrases,0.5));
                    }
                }
            } else if((*it) == getCountWithDuplication()-2) {
                KTYPE phrases = words[(*it)];
                if(phrases != root->data.key) {
                    if(find(vector.begin(), vector.end(), phrases) == vector.end()) {
                        countPhrases++;
                        vector.push_back(phrases);
                        root->data.includeKey.insert (pair<string ,double>(phrases,0.5));
                    }
                }
                phrases = phrases +" "+words[(*it)+1];
                if (find(vector.begin(), vector.end(), phrases) == vector.end()) {
                    countPhrases++;
                    vector.push_back(phrases);
                    root->data.includeKey.insert (pair<string ,double>(phrases,0.5));
                }
            }
        }
        traversal(words,vector, root->right);
    } // if
    return;
}	//  traversal


/*	=================== AVL_Empty ==================
	Returns true if tree is empty, false if any data.
	   Pre      tree has been created; may be null
	   Returns  true if tree empty, false if any data
*/

template <class TYPE, class KTYPE>
bool   AvlTree<TYPE, KTYPE> ::  AVL_Empty (void)
{
//	Statements
    return (count == 0);
}	//  AVL_Empty

/*	=================== AVL_Full ===================
	If there is no room for another node, returns true.
	   Pre      tree has been created
	   Returns  true if no room, false if room
*/

template <class TYPE, class KTYPE>
bool   AvlTree<TYPE, KTYPE> ::  AVL_Full (void)
{
//	Local Definitions
    NODE<TYPE>  *newPtr;

//	Statements
    newPtr = new NODE<TYPE>;
    if (newPtr)
    {
        delete  newPtr;
        return false;
    } // if
    else
        return true;
}	//  AVL_Full

/*	=================== AVL_Count ===================
	Returns number of nodes in tree.
	   Pre     tree has been created
	   Returns tree count
*/

template <class TYPE, class KTYPE>
int  AvlTree<TYPE, KTYPE> ::  AVL_Count (void)
{
// 	Statements
    return (count);
}	//  AVL_Count

/*	=================== Destructor ===================
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in inorder sequence.
	   Pre      tree is a pointer to a valid tree
	   Post     all data have been deleted
*/

template <class TYPE, class KTYPE>
AvlTree<TYPE, KTYPE> :: ~AvlTree  (void)
{
//	Statements
    if (tree)
        _destroyAVL (tree);
}	// Destructor

/*	=================== _destroyAVL ===================
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in postorder sequence.
	   Pre   tree is being destroyed
	   Post  all data have been deleted
*/

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> :: _destroyAVL (NODE<TYPE>  *root)
{
//	Statements
    if (root)
    {
        _destroyAVL (root->left);
        _destroyAVL (root->right);
        delete root;
    } // if
    return;
}	//  _destroyAVL

/*  ============================= AVL_Print =============================
	This function prints the AVL tree by calling a recursive inorder
	traversal. NOTE: THIS IS NOT AN APPLICATION ADT FUNCTION. IT IS
	USED ONLY FOR DEBUGGING PURPOSES.

	To correctly visualize the tree when turned sideways, the actual
	traversal is RNL.
	Pre	 Tree must be initialized. Null tree is OK.
		 Level is node level: root == 0
	Post Tree has been printed.
*/
template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> :: AVL_Print (void)
{
/*  statements */
    _print (tree, 0);
    return;
}   /* AVL_PRINT */

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> :: AVL_PrintWord(void)
{
/*  statements */
    print (tree, 0);
    return;
}

/*  ============================= _print =============================
	This function is not a part of the ADT. It is included to verify
	that the tree has been properly built by printing out the tree
	structure.
*/

/*  This function uses recursion to print the tree. At each level, the
    level number is printed along with the node contents (an integer).
    Pre		root is the root of a tree or subtree
            level is the level of the tree: tree root is 0
    Post    Tree has been printed.
*/
template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> :: _print (NODE<TYPE> *root,int level)
{
    /* Local Definitions */
    int i;

    /* Statements */
    if (root)
    {
        _print ( root->right, level + 1 );

        cout << "bal "     << setw(3) << root->bal
             << ": Level " << setw(3) << level;

        for (i = 0; i <= level; i++ )
            cout << "....";
        cout << setw(3) << root->data.key;
        if (root->bal == LH)
            cout << " (LH)\n";
        else if (root->bal == RH)
            cout << " (RH)\n";
        else
            cout << " (EH)\n";

        _print ( root->left, level + 1 );
    } /* if */

} /* AVL_Print */

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> :: print (NODE<TYPE> *root,int level)
{
    /* Local Definitions */
    int i;

    /* Statements */
    if (root)
    {
        print ( root->right, level + 1 );

        cout << "bal "     << setw(3) << root->bal
             << ": Level " << setw(3) << level;

        for (i = 0; i <= level; i++ )
            cout << "....";
        cout << setw(3) << root->data.word;
        if (root->bal == LH)
            cout << " (LH)\n";
        else if (root->bal == RH)
            cout << " (RH)\n";
        else
            cout << " (EH)\n";

        print ( root->left, level + 1 );
    } /* if */

} /* AVL_Print */

#endif //ASSIGNMENT2_DSA__AVL_ADT_H
