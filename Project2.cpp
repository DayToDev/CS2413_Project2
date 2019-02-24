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
	SparseMatrix<DT>* operator+(SparseMatrix& M);
	SparseMatrix<DT>* operator*(SparseMatrix& M);
	SparseMatrix<DT>* operator!();
	vector <SparseRow<DT> >* getMyMatrix();
	int getNoRows();
	int getNoCols();
	void setNoRows(int cols);
	void setNoCols(int cols);
	void setSparseRow(int pos, int row, int col, DT& val);
	void display(); //Display the sparse matrix
	void displayMatrix(); //Display matrix in original format
};

template <class DT>
SparseMatrix<DT>::SparseMatrix()
{
	CV = 0;
	noRows = -1;
	noCols = -1;
	myMatrix = new vector<SparseRow <DT> >();
}

template <class DT>
SparseMatrix<DT>::SparseMatrix(int nr, int nc, int cv)
{
	CV = cv;
	noRows = nr;
	noCols = nc;
	myMatrix = new vector<SparseRow <DT> >();
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
void SparseMatrix<DT>::setSparseRow(int pos, int row, int col, DT& val)
{
	SparseRow<DT>* tempSparse = new SparseRow<DT>*(row, col, val);
	myMatrix[pos] = tempSparse;
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
	for (unsigned int i = 0; i < myMatrix->size(); i++)
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
			for (unsigned int j = 0; j < myMatrix->size(); j++)
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
SparseMatrix<DT>* SparseMatrix<DT>::operator!()
{
	SparseMatrix<DT>* tempMatrix;
	tempMatrix = this->Transpose();
	return tempMatrix;
}

template <class DT>
SparseMatrix<DT>* SparseMatrix<DT>::Transpose()
{
	SparseMatrix<DT>* temp = new SparseMatrix(noRows, noCols, CV);
	SparseRow<DT>* tempRow;

	for (unsigned int i = 0; i < myMatrix->size(); i++)
	{
		tempRow = new SparseRow<DT>(myMatrix->at(i).getCol(), myMatrix->at(i).getRow(), myMatrix->at(i).getVal());
		(*temp).myMatrix->push_back(*tempRow);
	}

	return temp;
}

template <class DT>
SparseMatrix<DT>* SparseMatrix<DT>::operator+(SparseMatrix& M)
{
	SparseMatrix<DT>* tempMatrix;
	tempMatrix = Add(M);
	return tempMatrix;
}

template <class DT>
SparseMatrix<DT>* SparseMatrix<DT>::Add(SparseMatrix<DT>& M)
{
	SparseMatrix<DT>* temp = new SparseMatrix<DT>(noRows, noCols, CV);
	SparseRow<DT>* tempPtr;
	unsigned int i = 0;
	unsigned int j;
	int valueToAdd;
	int sum;
	bool found;

	bool* mArray = new bool[M.myMatrix->size()];
	for (unsigned int k = 0; k < M.myMatrix->size(); k++) mArray[k] = false;

	while (i < myMatrix->size())
	{
		j = 0;
		found = false;
		sum = 0;
		valueToAdd = 0;
		while ((j < M.myMatrix->size()) && (!found))
		{
			if ((myMatrix->at(i).getRow() == M.myMatrix->at(j).getRow()) && (myMatrix->at(i).getCol() == M.myMatrix->at(j).getCol()))
			{
				found = true;
				valueToAdd = M.myMatrix->at(j).getVal();
				mArray[j] = true;
			}
			else j++;
		}
		if (found)
		{
			sum = (myMatrix->at(i).getVal() + valueToAdd);
		}
		tempPtr = new SparseRow<DT>(myMatrix->at(i).getRow(), myMatrix->at(i).getCol(), sum);
		(*temp).myMatrix->push_back(*tempPtr);
		i++;
	}
	//Copy new SparseRow objects from M
	/*for (int k = 0; k < M.curSize; k++)
	{
		if (!mArray[k])
		{
			(*temp).myMatrix[(*temp).curSize].setRow(myMatrix[k].getRow());
			(*temp).myMatrix[(*temp).curSize].setCol(myMatrix[k].getCol());
			(*temp).myMatrix[(*temp).curSize].setVal(myMatrix[k].getVal());
		}
	}*/
	delete[] mArray;
	return temp;
}

template <class DT>
SparseMatrix<DT>* SparseMatrix<DT>::operator*(SparseMatrix& M)
{
	SparseMatrix<DT>* tempMatrix;
	tempMatrix = Multiply(M);
	return tempMatrix;
}

template <class DT>
SparseMatrix<DT>* SparseMatrix<DT>::Multiply(SparseMatrix<DT>& M)
{
	SparseMatrix<DT>* temp = new SparseMatrix<DT>(noRows, noCols, CV);
	SparseRow<DT>* tempPtr;
	int num1 = 0;
	int num2 = 0;
	int val1 = 0;
	int val2 = 0;
	unsigned int counter1 = 0;
	unsigned int counter2 = 0;
	for (int i = 0; i < noRows; i++)
	{
		for (int j = 0; j < M.noCols; j++)
		{
			for (int k = 0; k < M.noRows; k++) //Change to for loop
			{
				while (counter1 < myMatrix->size())
				{
					if (myMatrix->at(counter1).getRow() == i && myMatrix->at(counter1).getCol() == k)
					{
						num1 = myMatrix->at(counter1).getVal();
						counter1 = myMatrix->size(); //Exit loop immediately
					}
					counter1++;
				}

				while (counter2 < M.myMatrix->size())
				{
					if (M.myMatrix->at(counter2).getRow() == k && M.myMatrix->at(counter2).getCol() == j)
					{
						num2 = M.myMatrix->at(counter2).getVal();
						counter2 = M.myMatrix->size();
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
				tempPtr = new SparseRow<DT>(i, j, val2);
				(*temp).myMatrix->push_back(*tempPtr);
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
			if (v != cv)
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
			if (v != cv)
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

	cout << "After Transpose first one in normal format" << endl;
	tempMatrix = !(*firstMatrix);

	(*tempMatrix).displayMatrix();

	cout << "After Transpose second one in normal format" << endl;
	tempMatrix = !(*secondMatrix);
	(*tempMatrix).displayMatrix();
	
	cout << "Multiplication of matrices in sparse matrix format: " << endl;
	tempMatrix = (*secondMatrix) * (*firstMatrix);
	(*tempMatrix).display();

	cout << "Addition of matrices in sparse matrix format: " << endl;
	tempMatrix = (*secondMatrix) + (*firstMatrix);
	(*tempMatrix).display();
	
return 0;	
};