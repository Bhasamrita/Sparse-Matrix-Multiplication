#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <numeric>

using namespace std;

int main()
{
    //Declaration of variables
    unsigned int m, n, p, i, j, k, c, random, flag;
    double spa, spb;

    //We initialise the rand() function to the system time to make the values unique every time.
    srand((unsigned) time(0));

    /*Asking the user to input the dimensions as well as the sparsity of the two matrices.
      Sparsity = 1.0 means the matrix is empty, and
      Sparsity = 0.0 means the matrix is full. */
    cout << "Let matrix A be an mXn matrix and matrix B be an nXp matrix.\n";
    cout << "Enter the values of m, n, and p: ";
    cin >> m >> n >> p;
    cout << "Enter the sparsity of the matrices A and B respectively: ";
    cin >> spa >> spb;

    //Calculating the density of the two matrices.
    spa = 1.0 - spa;
    spb = 1.0 - spb;

    /*We have 3 vectors for each of the matrices -
      row vector consists of randomly generated row values for the non-zero elements
      col vector consists of randomly generated column values for the non-zero elements
      val vector consists of randomly generated values for the elements themselves
      The elements are restricted to hold values from 1 to 10 just for the sake of simplicity.*/
    vector<int> rowa, cola, vala, rowb, colb, valb;

    /*After multiplication we store the values in 3 vectors -
      mul for storing multiplied values, mulrow for storing the corresponding row numbers,
      and mulcol for storing the corresponding column numbers. */
    vector<int> mulrow, mulcol, mul;

    /*Matrix A will contain ceil(m*n*spa) non-zero elements. We create the row numbers randomly.
      Matrix A will have row numbers from 0 to m-1. */
    for (i = 0; i < m*n*spa; i++)
    {
        random = rand() % m;
        rowa.push_back(random);
    }

    /*Matrix B will contain ceil(n*p*spb) non-zero elements. We create the row numbers randomly.
      Matrix B will have row numbers from 0 to n-1. */
    for (i = 0; i < n*p*spb; i++)
    {
        random = rand() % n;
        rowb.push_back(random);
    }

    /*We sort the random row values from both the matrices so as to get
      the elements in their increasing order of positions. */
    sort(rowa.begin(), rowa.end());
    sort(rowb.begin(), rowb.end());

    /*Two vectors vec_n and vec_p are created to hold all possible distinct column values.
      vec_n is of length n and contains elements from 0 to n-1 in increasing order.
      vec_p is of length p and contains elements from 0 to p-1 in increasing order. */
    vector<int> vec_n(n), vec_p(p);
    iota(vec_n.begin(), vec_n.end(), 0);
    iota(vec_p.begin(), vec_p.end(), 0);

    /*The col values created are shuffled so as to get random values.
      The col values are created this way instead of using rand() because
      the values tend to repeat when the latter is used.
      As a result, we may end up getting the same position twice or more.
      E.g. (0,0) may occur 2 times and (2,3) may occur 3 times, and so on. */
    random_shuffle(vec_n.begin(), vec_n.end());
    random_shuffle(vec_p.begin(), vec_p.end());

    /*For each row in matrix A we insert the randomly created column numbers
      and sort them in ascending order.
      E.g. (0,1) will always appear before (0,4) or (3,7). */
    c = 0, j = 0;
    for(i = 0; i < (m*n*spa)-1; i++)
    {
        k = 0;
        while(rowa[i] == rowa[i+1])
        {
            cola.push_back(vec_n[k]);
            c++; i++; k++;
        }
        cola.push_back(vec_n[k]);
        random_shuffle(vec_n.begin(), vec_n.end());
        c++;
        sort(cola.begin()+j, cola.begin()+c);
        j = c;
    }

    /*For each row in matrix B we insert the randomly created column numbers
      and sort them in ascending order. */
    c = 0, j = 0;
    for(i = 0; i < (n*p*spb)-1; i++)
    {
        k = 0;
        while(rowb[i] == rowb[i+1])
        {
            colb.push_back(vec_p[k]);
            c++; i++; k++;
        }
        colb.push_back(vec_p[k]);
        random_shuffle(vec_p.begin(), vec_p.end());
        c++;
        sort(colb.begin()+j, colb.begin()+c);
        j = c;
    }

    //The randomly generated values for the elements of matrix A.
    for (i = 0; i < m*n*spa; i++)
    {
        random = (rand() % 10) + 1;
        vala.push_back(random);
    }

    //The randomly generated values for the elements of matrix B.
    for (i = 0; i < n*p*spb; i++)
    {
        random = (rand() % 10) + 1;
        valb.push_back(random);
    }

    for (i = 0; i < m*n*spa; i++)
        cout << rowa[i] << " " << cola[i] << " " << vala[i] << "\n";
    cout << "\n";

    for (i = 0; i < n*p*spb; i++)
        cout << rowb[i] << " " << colb[i] << " " << valb[i] << "\n";
    cout << "\n";


    /*There is no need for a compatibility check before doing multiplication because
      we have already assumed that A is of dimensions mXn and B is nXp
      and have taken the inputs accordingly. */

    /*Multiplying the elements that have matching column number in matrix A and
      row number in matrix B. The resultant product will have its corresponding
      row and column numbers as the row number of matrix A and the column number of matrix B.
      E.g. multiplying elements (1, 8) and (8,3) will generate a partial product
      which will be placed at (1, 3) in the resultant matrix.
      There may be multiple elements with the same row and column numbers.
      E.g. (1, 4) and (4, 2) will generate a partial product for (1,2).
      Likewise, (1, 7) and (7, 2) will also generate a partial product for (1,2).
      Currently, these two partial products will occupy different positions in the mul vectors. */
    for (i = 0; i < m*n*spa; i++)
        for (j = 0; j < n*p*spb;)
        {
            if (cola[i] == rowb[j])
            {
                mul.push_back(vala[i] * valb[j]);
                mulrow.push_back(rowa[i]);
                mulcol.push_back(colb[j]);
                j++;
            }
            else if (cola[i] < rowb[j])
                break;
            else if (cola[i] > rowb[j])
                j++;
        }

    /*Adding up the products that belong to the same row and column and
      deleting the redundant entries. Continuing from the above example,
      (1, 2) can be found in two places. We add up the second partial product
      with the first, and delete the entries holding the second partial product. */
    for (i = 0; i < mul.size(); i++)
    {
        for (j = i+1; j < mul.size(); j++)
        {
            flag = 0;
            if ((mulrow[i] == mulrow[j]) && (mulcol[i] == mulcol[j]))
            {
                mul[i] = mul[i] + mul[j];
                mul.erase(mul.begin() + j);
                mulrow.erase(mulrow.begin() + j);
                mulcol.erase(mulcol.begin() + j);
                flag = 1;
            }
            else if (mulrow[i] < mulrow[j])
                break;

            if (flag == 1)
                j--;
        }
    }

    /*The final matrix will contain the following entries with
      their corresponding values as displayed.
      The entries are displayed in their increasing order of row numbers but
      the column numbers with a single row may be mixed up.
      E.g. (0, 2) may appear before (0,1) but (1, 1) will definitely be after (0, 5). */
    cout << "The final entries of the matrix after multiplying the two matrices is as follows:\n";
    for (i = 0; i < mul.size(); i++)
        cout << mulrow[i] << " " << mulcol[i] << " " << mul[i] << "\n";

    return 0;
}
