#include <iostream>
#include <vector>
using namespace std;

template <class DT>
class SparseRow
{
protected:
	int r; //Row position
	int c; //Column position
	DT v; //Value

public:
	SparseRow(); //Default constructor
	SparseRow(int row, int col, DT& value);
	void display(); //Prints Row#, Column#, Value
	int getRow(); 
	int getCol();
	DT& getVal();
	void setRow(int row);
	void setCol(int col);
	void setVal(DT& val);

};

template <class DT>
SparseRow<DT>::SparseRow()
{
	r = -1;
	c = -1;
	v = 0;
}

template <class DT>
SparseRow<DT>::SparseRow(int row, int col, DT& val)
{
	r = row;
	c = col;
	v = val;
}

template <class DT>
int SparseRow<DT>::getRow()
{
	return r;
}

template <class DT>
int SparseRow<DT>::getCol()
{
	return c;
}

template <class DT>
DT& SparseRow<DT>::getVal()
{
	return v;
}

template <class DT>
void SparseRow<DT>::setRow(int row)
{
	r = row;
}

template <class DT>
void SparseRow<DT>::setCol(int col)
{
	c = col;
}

template <class DT>
void SparseRow<DT>::setVal(DT& val)
{
	v = val;
}

template <class DT>
void SparseRow<DT>::display()
{
	cout << r << ", " << c << ", " << v << endl;
}

template <class DT>
class SparseMatrix
{
protected:
	int CV; //Common Value
	int noRows; //Number of rows
	int noCols; //Number of columns
	vector <SparseRow<DT> >* myMatrix; //Pointer to created matrix

public:
	SparseMatrix(); //Default constructor
	SparseMatrix(int nr, int nc, int cv);
	SparseMatrix<DT>* Transpose(); //Transposes a matrix diagonally
	SparseMatrix<DT>* Multiply(SparseMatrix& M); //Multiplies two matrices and returns its resultant
	SparseMatrix<DT>* Add(SparseMatrix& M); //Adds two matricies and returns its result
	vector <SparseRow<DT> >* getMyMatrix();
	int getNoRows();
	int getNoCols();
	void setNoRows(int cols);
	void setNoCols(int cols);
	void display(); //Display the sparse matrix
	void displayMatrix(); //Display matrix in original format
};

template <class DT>
SparseMatrix<DT>::SparseMatrix()
{
	CV = 0;
	noRows = -1;
	noCols = -1;
	vector<SparseRow <DT> >* myMatrix (10);
}

template <class DT>
SparseMatrix<DT>::SparseMatrix(int nr, int nc, int cv)
{
	CV = cv;
	noRows = nr;
	noCols = nc;
	vector<SparseRow <DT> >* myMatrix (10);
}

template <class DT>
vector<SparseRow<DT> >* SparseMatrix<DT>::getMyMatrix()
{
	return myMatrix;
}

template <class DT>
int SparseMatrix<DT>::getNoRows()
{
	return noRows;
}

template <class DT>
int SparseMatrix<DT>::getNoCols()
{
	return noCols;
}

template <class DT>
void SparseMatrix<DT>::setNoRows(int rows)
{
	noRows = rows;
}

template <class DT>
void SparseMatrix<DT>::setNoCols(int cols)
{
	noCols = cols;
}

template <class DT>
void SparseMatrix<DT>::display()
{
	for (int i = 0; i < myMatrix->size(); i++)
	{
		if (myMatrix->at(i).getVal() != CV)
			myMatrix->at(i).display();
	}
}

template <class DT>
void SparseMatrix<DT>::displayMatrix()
{
	int val = 0;
	for (int i = 0; i < noRows; i++)
	{
		for (int k = 0; k < noCols; k++)
		{
			for (int j = 0; j < myMatrix->size(); j++)
			{
				if ((i == myMatrix->at(j).getRow()) && (k == myMatrix->at(j).getCol()))
				{
					val = myMatrix->at(j).getVal();
					cout << val << " ";
				}
			}
			if (val == 0)
			{
				cout << val << " ";
			}
			val = 0;
		}
		cout << endl;
	}

}

template <class DT>
SparseMatrix<DT>* SparseMatrix<DT>::Transpose()
{
	SparseMatrix<DT>* temp = new SparseMatrix(noRows, noCols, CV);
	SparseRow<DT>* tempRow;

	for (int i = 0; i < myMatrix.size; i++)
	{
		tempRow = new SparseRow(myMatrix[i].getCol(), myMatrix[i].getRow(), myMatrix[i].getVal());
		(*temp).myMatrix[i] = *tempRow;
	}

	return temp;
}

template <class DT>
SparseMatrix<DT>* SparseMatrix<DT>::Add(SparseMatrix<DT>& M)
{
	SparseMatrix<DT>* temp = new SparseMatrix(noRows, noCols, CV, myMatrix.size + M.myMatrix.size);
	int i = 0;
	int j;
	int valueToAdd;
	bool found;

	bool* mArray = new bool[M.curSize];
	for (int k = 0; k < M.curSize; k++) mArray[k] = false;

	while (i < myMatrix.size)
	{
		j = 0;
		found = false;
		(*temp).myMatrix[(*temp).curSize].setRow(myMatrix[i].getRow());
		(*temp).myMatrix[(*temp).curSize].setCol(myMatrix[i].getCol());
		valueToAdd = 0;
		while ((j < M.curSize) && (!found))
		{
			if ((myMatrix[i].getRow() == M.myMatrix[j].getRow()) && (myMatrix[i].getCol() == M.myMatrix[j].getCol()))
			{
				found = true;
				valueToAdd = M.myMatrix[j].getVal();
				mArray[j] = true;
			}
			else j++;
		}
		(*temp).myMatrix[(*temp).curSize++].setVal(myMatrix[i].getVal() + valueToAdd);
		i++;
	}
	//Copy new SparseRow objects from M
	for (int k = 0; k < M.curSize; k++)
	{
		if (!mArray[k])
		{
			(*temp).myMatrix[(*temp).curSize].setRow(myMatrix[k].getRow());
			(*temp).myMatrix[(*temp).curSize].setCol(myMatrix[k].getCol());
			(*temp).myMatrix[(*temp).curSize].setVal(myMatrix[k].getVal());
		}
	}
	delete[] mArray;
	return temp;
}

template <class DT>
SparseMatrix<DT>* SparseMatrix<DT>::Multiply(SparseMatrix<DT>& M)
{
	SparseMatrix* temp = new SparseMatrix(noRows, noCols, CV, myMatrix.size + M.myMatrix.size);
	SparseRow* tempPtr;
	int num1 = 0;
	int num2 = 0;
	int val1 = 0;
	int val2 = 0;
	int counter1 = 0;
	int counter2 = 0;
	for (int i = 0; i < noRows; i++)
	{
		for (int j = 0; j < M.noCols; j++)
		{
			for (int k = 0; k < M.noRows; k++) //Change to for loop
			{
				while (counter1 < myMatrix.size)
				{
					if (myMatrix[counter1].getRow() == i && myMatrix[counter1].getCol() == k)
					{
						num1 = myMatrix[counter1].getVal();
						counter1 = myMatrix.size; //Exit loop immediately
					}
					counter1++;
				}

				while (counter2 < M.myMatrix.size)
				{
					if (M.myMatrix[counter2].getRow() == k && M.myMatrix[counter2].getCol() == j)
					{
						num2 = M.myMatrix[counter2].getVal();
						counter2 = M.myMatrix.size;
					}
					counter2++;
				}

				if (num1 != 0 && num2 != 0)
				{
					val1 += num1 * num2;
					num1 = 0;
					num2 = 0;
					counter1 = 0;
					counter2 = 0;
				}
				if (val1 != 0)
				{
					val2 += val1;
					val1 = 0;
				}
			}
			if (val2 != 0)
			{
				tempPtr = new SparseRow(i, j, val2);
				(*temp).myMatrix.push_back(*tempPtr);
			}
			//Reset for next loop
			val1 = 0;
			num1 = 0;
			num2 = 0;
			val2 = 0;
			counter1 = 0;
			counter2 = 0;
		}
	}
	return temp;
}


int main()
{
	int nr, nc, cv, size;
	int v;
	int c = 0;
	int r = 0;

	SparseMatrix<int>* tempMatrix;
	SparseMatrix<int>* firstMatrix;
	SparseMatrix<int>* secondMatrix;

	SparseRow<int>* tempRow;
	SparseRow<int>* tempRow2;

	cin >> nr >> nc >> cv >> size; //Reads in parameters for first Matrix
	firstMatrix = new SparseMatrix<int>(nr, nc, cv);
	while (c < nc)
	{
		while (r < nr)
		{
			cin >> v;
			if (v != 0)
			{
				tempRow = new SparseRow<int>(c, r, v);
				(*firstMatrix).getMyMatrix()->push_back(*tempRow);
			}
			r++;
		}
		r = 0;
		c++;
	}
	
	cin >> nr >> nc >> cv >> size; //Reads in parameters for second Matrix
	secondMatrix = new SparseMatrix<int>(nr, nc, cv);

	r = 0;
	c = 0;
	while (c < nc)
	{
		while (r < nr)
		{
			cin >> v;
			if (v != 0)
			{
				tempRow2 = new SparseRow<int>(c, r, v);
				(*secondMatrix).getMyMatrix()->push_back(*tempRow2);
			}
			r++;
		}
		r = 0;
		c++;
	}

	cout << "First one in sparse matrix format" << endl;
	(*firstMatrix).display();

	cout << "First one in normal matrix format" << endl;
	(*firstMatrix).displayMatrix();

	cout << "Second one in sparse matrix format" << endl;
	(*secondMatrix).display();

	cout << "Second one in normal matrix format" << endl;
	(*secondMatrix).displayMatrix();

	/*cout << "After Transpose first one in normal format" << endl;
	tempMatrix = (*firstMatrix).Transpose();

	(*tempMatrix).displayMatrix();

	cout << "After Transpose second one in normal format" << endl;
	tempMatrix = (*secondMatrix).Transpose();
	(*tempMatrix).displayMatrix();
	
	cout << "Multiplication of matrices in sparse matrix format: " << endl;
	tempMatrix = (*secondMatrix).Multiply(*firstMatrix);
	(*tempMatrix).display();

	cout << "Addition of matrices in sparse matrix format: " << endl;
	tempMatrix = (*secondMatrix).Add(*firstMatrix);
	(*tempMatrix).display();
	*/
return 0;	
};