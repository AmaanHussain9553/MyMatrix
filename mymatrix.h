/*mymatrix.h*/

//
// Amaan Hussain
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #02
//

//
// mymatrix
//
// The mymatrix class provides a matrix (2D array) abstraction.
// The size can grow dynamically in both directions (rows and
// cols).  Also, rows can be "jagged" --- i.e. rows can have
// different column sizes, and thus the matrix is not necessarily
// rectangular.  All elements are initialized to the default value
// for the given type T.  Example:
//
//   mymatrix<int>  M;  // 4x4 matrix, initialized to 0
//
//   M(0, 0) = 123;
//   M(1, 1) = 456;
//   M(2, 2) = 789;
//   M(3, 3) = -99;
//
//   M.growcols(1, 8);  // increase # of cols in row 1 to 8
//
//   for (int r = 0; r < M.numrows(); ++r)
//   {
//      for (int c = 0; c < M.numcols(r); ++c)
//         cout << M(r, c) << " ";
//      cout << endl;
//   }
//
// Output:
//   123 0 0 0
//   0 456 0 0 0 0 0 0
//   0 0 789 0
//   0 0 0 -99
//

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

template<typename T>
class mymatrix
{
private:
    struct ROW
    {
        T*  Cols;     // dynamic array of column elements
        int NumCols;  // total # of columns (0..NumCols-1)
    };

    ROW* Rows;     // dynamic array of ROWs
    int  NumRows{};  // total # of rows (0..NumRows-1)
    int Size = 0;
public:
    //
    // default constructor:
    //
    // Called automatically by C++ to construct a 4x4 matrix.  All
    // elements are initialized to the default value of T.
    //
    mymatrix()
    {
        Size = 0;
        Rows = new ROW[4];  // an array with 4 ROW structs:
        NumRows = 4;

        // initialize each row to have 4 columns:
        for (int r = 0; r < NumRows; ++r)
        {
            Rows[r].Cols = new T[4];  // an array with 4 elements of type T:
            Rows[r].NumCols = 4;

            // initialize the elements to their default value:
            for (int c = 0; c < Rows[r].NumCols; ++c)
            {
                Rows[r].Cols[c] = T{};  // default value for type T:
            }
        }
    }


    //
    // parameterized constructor:
    //
    // Called automatically by C++ to construct a matrix with R rows,
    // where each row has C columns. All elements are initialized to
    // the default value of T.
    //
    mymatrix(int R, int C)
    {
        Size = 0;
        if (R < 1) {
            throw invalid_argument("mymatrix::constructor: # of rows");
        }
        if (C < 1) {
            throw invalid_argument("mymatrix::constructor: # of cols");
        }
        Rows = new ROW[R];  // an array with 4 ROW structs:
        NumRows = R;

        // initialize each row to have 4 columns:
        for (int r = 0; r < NumRows; ++r)
        {
            Rows[r].Cols = new T[C];  // an array with 4 elements of type T:
            Rows[r].NumCols = C;
            // initialize the elements to their default value:
            for (int c = 0; c < C; ++c)
            {
                Rows[r].Cols[c] = T{};  // default value for type T:
            }
        }
        Size = size();
    }


    //
    // copy constructor:
    //
    // Called automatically by C++ to construct a matrix that contains a
    // copy of an existing matrix.  Example: this occurs when passing
    // mymatrix as a parameter by value
    //
    //   void somefunction(mymatrix<int> M2)  <--- M2 is a copy:
    //   { ... }
    //
     mymatrix(mymatrix<T>& other)
     {
        /* this->Rows = new ROW[other.Size];
         this->Size = other.Size;
         this->NumRows = other.NumRows;

         for(int i = 0; i < other.NumRows; i++){
             for(int j = 0; i < other.Rows[i].NumCols; j++){
                 other.Rows[i].Cols[j] = T{};//iniializing all values to 0
             }
         }
         for(int i = 0; i < NumRows; i++){
             for(int j = 0; i < Rows[i].NumCols; j++)
             other.Rows[i].Cols[j] = Rows[i].Cols[j];//copying all values from initial matrix to copy matrix
         }*/

     }


    //
    // numrows
    //
    // Returns the # of rows in the matrix.  The indices for these rows
    // are 0..numrows-1.
    //
    int numrows() const
    {
        return NumRows;
    }


    //
    // numcols
    //
    // Returns the # of columns in row r.  The indices for these columns
    // are 0..numcols-1.  Note that the # of columns can be different
    // row-by-row since "jagged" rows are supported --- matrices are not
    // necessarily rectangular.
    //
    int numcols(int r) const
    {
        if (r < 0 || r >= NumRows)
            throw invalid_argument("mymatrix::numcols: row");

        return Rows[r].NumCols;
    }


    //
    // growcols
    //
    // Grows the # of columns in row r to at least C.  If row r contains
    // fewer than C columns, then columns are added; the existing elements
    // are retained and new locations are initialized to the default value
    // for T.  If row r has C or more columns, then all existing columns
    // are retained -- we never reduce the # of columns.
    //
    // Jagged rows are supported, i.e. different rows may have different
    // column capacities -- matrices are not necessarily rectangular.
    //
    void growcols(int r, int C)
    {
        Size = 0;
        if (r < 0 || r >= NumRows)
            throw invalid_argument("mymatrix::growcols: row");
        if (C < 1)
            throw invalid_argument("mymatrix::growcols: columns");

        if(C <= Rows[r].NumCols){
            return;
        }
        T* newCol = new T[C];

        for(int i = 0; i<Rows[r].NumCols; i++){
            newCol[i] = Rows[r].Cols[i];
        }
        for(int j = Rows[r].NumCols; j < C; j++){
            newCol[j] = T{};
        }
        delete[] Rows[r].Cols;
        Rows[r].Cols = newCol;
        Rows[r].NumCols =C;

    }


    //
    // grow
    //
    // Grows the size of the matrix so that it contains at least R rows,
    // and every row contains at least C columns.
    //
    // If the matrix contains fewer than R rows, then rows are added
    // to the matrix; each new row will have C columns initialized to
    // the default value of T.  If R <= numrows(), then all existing
    // rows are retained -- we never reduce the # of rows.
    //
    // If any row contains fewer than C columns, then columns are added
    // to increase the # of columns to C; existing values are retained
    // and additional columns are initialized to the default value of T.
    // If C <= numcols(r) for any row r, then all existing columns are
    // retained -- we never reduce the # of columns.
    //
    void grow(int R, int C)
    {
        Size = 0;
        if (R < 1)
            throw invalid_argument("mymatrix::grow: # of rows");
        if (C < 1)
            throw invalid_argument("mymatrix::grow: # of cols");
        int tempRow = NumRows;//setting a temp value for NumRows to know until
                              //where should values be retained
        ROW *Rows1 = new ROW[R];//creating new struct to add more rows
        if(R > NumRows && C > Rows[0].NumCols) {
            for (int r = 0; r < tempRow; r++) {
                growcols(r, C);
                Rows1[r] = Rows[r];
            }
            for(int s = tempRow; s < R; s++){
                Rows1[s].Cols = new T[C];//creating row struct of new column size
                Rows1[s].NumCols = C;
            }
            for(int t = tempRow; t < R; t++){
                    for (int f = 0; f < Rows1[t].NumCols; f++)
                        Rows1[t].Cols[f] = T{};//initializing new row structs
            }
            delete [] Rows;//deleting old rows
            Rows = Rows1;
            NumRows = R;
        }
        else if(R <= NumRows && C > Rows[0].NumCols){

            for (int r = 0; r < NumRows; r++) {
                growcols(r, C);
                Rows[r].NumCols = C;
            }
        }
        else if(R > NumRows && C <= Rows[0].NumCols){
            for (int r = 0; r < tempRow; r++) {
                Rows1[r] = Rows[r];
            }

            for(int s = tempRow; s < R; s++){
                Rows1[s].Cols = new T[Rows[0].NumCols];//creating row struct of new column size
                Rows1[s].NumCols = Rows[0].NumCols;
            }
            for(int t = tempRow; t < NumRows; t++){
                    for (int f = 0; f < Rows1[t].NumCols; f++)
                        Rows1[t].Cols[f] = T{};//initializing new row structs
            }
            delete [] Rows;//deleting old rows
            Rows = Rows1;
            NumRows = R;
        }
    }


    //THis function is able to find the GENERAL number
    //of columns even after certain rows have gone
    //through the growcols function
    //It is helpful of scalar multiplication and matrix
    //multiplication function when result has to be initialized
    //to a size
    int findSmallestColumnSize(){
        int smallest = Rows[0].NumCols;
        for(int r = 0; r < NumRows; r++){
            if(smallest > Rows[r].NumCols){
                smallest = Rows[r].NumCols;
            }
        }
        return smallest;
    }


    //
    // size
    //
    // Returns the total # of elements in the matrix.
    //
    int size() const
    {
        int Size = 0;
        for(int r = 0; r < NumRows; r++ ){
            //cout << numcols(r) << endl;
            Size += Rows[r].NumCols;
        }
        return Size;
    }


    //
    // at
    //
    // Returns a reference to the element at location (r, c); this
    // allows you to access the element or change it:
    //
    //    M.at(r, c) = ...
    //    cout << M.at(r, c) << endl;
    //
    T& at(int r, int c)
    {
        if (r < 0 || r >= NumRows)
            throw invalid_argument("mymatrix::at: row");
        if (c < 0 || c >= Rows[r].NumCols)
            throw invalid_argument("mymatrix::at: col");

        return Rows[r].Cols[c];
    }


    //
    // ()
    //
    // Returns a reference to the element at location (r, c); this
    // allows you to access the element or change it:
    //
    //    M(r, c) = ...
    //    cout << M(r, c) << endl;
    //
    T& operator()(int r, int c)
    {
        if (r < 0 || r >= NumRows)
            throw invalid_argument("mymatrix::(): row");
        if (c < 0 || c >= Rows[r].NumCols)
            throw invalid_argument("mymatrix::(): col");

        return Rows[r].Cols[c];
    }


    //
    // scalar multiplication
    //
    // Multiplies every element of this matrix by the given scalar value,
    // producing a new matrix that is returned.  "This" matrix is not
    // changed.
    //
    // Example:  M2 = M1 * 2;
    //
    mymatrix<T> operator*(T scalar)
    {
        mymatrix<T> result(NumRows,findSmallestColumnSize());
        //initialized size of result
        result.NumRows = NumRows;
        for (int r = 0; r < NumRows; r++) {
            result.growcols(r, Rows[r].NumCols);
            for (int c = 0; c < Rows[r].NumCols; c++) {
                result(r, c) = T{};//initializing variables in result
                                   //after result is made same as actual matrix
            }
        }

        for(int r = 0; r < result.NumRows; r++){
            for(int c = 0; c < Rows[r].NumCols; c++){
                result(r,c) = Rows[r].Cols[c] * scalar;//scalar multiplication
            }
        }
        return result;
    }


    //
    // matrix multiplication
    //
    // Performs matrix multiplication M1 * M2, where M1 is "this" matrix and
    // M2 is the "other" matrix.  This produces a new matrix, which is returned.
    // "This" matrix is not changed, and neither is the "other" matrix.
    //
    // Example:  M3 = M1 * M2;
    //
    // NOTE: M1 and M2 must be rectangular, if not an exception is thrown.  In
    // addition, the sizes of M1 and M2 must be compatible in the following sense:
    // M1 must be of size RxN and M2 must be of size NxC.  In this case, matrix
    // multiplication can be performed, and the resulting matrix is of size RxC.
    //
    mymatrix<T> operator*(mymatrix<T>& other)
    {

        //
        // both matrices must be rectangular for this to work:
        //
        // if (this matrix is not rectangular)
        //   throw runtime_error("mymatrix::*: this not rectangular");
        int columnSize1 = Rows[0].NumCols;
        for(int r = 0; r < NumRows; r++){
            if(Rows[r].NumCols != columnSize1){
                throw runtime_error("mymatrix::*: this not rectangular");
            }
        }
        //
        // if (other matrix is not rectangular)
        //   throw runtime_error("mymatrix::*: other not rectangular")
        int columnSize2 = other.Rows[0].NumCols;
        for(int r = 0; r < other.NumRows; r++){
            if(other.NumRows != columnSize2){
                throw runtime_error("mymatrix::*: other not rectangular");
            }
        }
        //
        // Okay, both matrices are rectangular.  Can we multiply?  Only
        // if M1 is R1xN and M2 is NxC2.  This yields a result that is
        // R1xC2.
        //
        // Example: 3x4 * 4x2 => 3x2
        //
        if(Rows[0].NumCols != other.NumRows){
            throw runtime_error("mymatrix::*: size mismatch");
        }
        //Beginning of creating result matrix and multiplying
        //First to initialize all variables in result
        mymatrix<T> result(Rows[0].NumCols,other.NumRows);
        for (int r = 0; r < Rows[0].NumCols; r++) {
            for (int c =Rows[0].NumCols ; c < other.NumRows; c++) {
                result(r, c) = T{};//intializing to default
            }
        }
        result.NumRows = (this->NumRows);
        int value = 0;
        for(int i = 0; i < NumRows; ++i) {//goes through the rows after end of one row multiplication
            for (int j = 0; j < result.Rows[0].NumCols; ++j) {//goes through each column in other matrix
                for (int k = 0; k < other.NumRows; ++k) {// keeps RowNumber of actual matrix
                                                         //and columnNumber of other matrix same
                                                         //as multiplication goes on
                    value += (Rows[i].Cols[k] * other.at(k,j));
                }
                result(i, j) = value;
                value = 0;//resetting value to 0
            }

        }

        return result;
    }

    //
    // _output
    //
    // Outputs the contents of the matrix; for debugging purposes.
    //
    void _output()
    {
        for (int r = 0; r < this->NumRows; ++r)
        {
            for (int c = 0; c < this->Rows[r].NumCols; ++c)
            {
                cout << this->Rows[r].Cols[c] << " ";
            }
            cout << endl;
        }
    }

};