/***********************************************************************************************************************
 * Student name: Meech Lamirande Kabuasa
 * Student number: 040880736
 * Version: 1.0
 * Assignment number: 2
 * Course name and number CST8233_300
 * Lab section number: 302
 * Professor’s name: Dr Hala Own
 * Due date of assignment: March 15th, 2021
 * Submission date of assignment:
 * List of source and header files in the project:
 *      ->Main.cpp
 *
 * DESCRIPTION OF THE PROBLEM:
 * ----------------------------------------------
 *       y = ae^bx
 *
 *       take Natural Log of both sides.
 *
 *       For the data in the file, the function has the form N = ae ^ bx  a and  b are constants that are determined from the fit.
 *       Until the user wishes to quit the application
 *       should do the following:
 *
 *       •Read data from a file
 *       •Print the data
 *       •Print report summary of the data (mean, median, range, mood, and standard Deviation)
 *       •Do an exponential fitand print the regression equation
 *       •Compute how accurate your exponential fit (Standard Error)
 *       •Offer an interpolation/extrapolation of both theprognostic index


  Example File.
  * * * * * * * * * * * * * * * * * * * *
  |    i    Days	Prognostic_index
  * * * * * * * * * * * * * * * * * * * *
       0     2	    54
       1     5	    50
       2     7	    45
       3     10	    37
       4     14	    35
       5     19	    25
       6     26	    20
       7     31	    16
       8     34	    18
       9     38	    13
       10    45	    8
       11    52	    11
       12    53	    8
       13    60	    4
       14    65	    6

 *  TODO what needs to be implemented {
 *   Main menu 2 choices
 *    > Exponential fit || > Quite -> EXIT;
 *    > Exponential fit ? create File handler > Check no file? >
 *   File Reader to read data line by line.
 *    > first get number of lines to determine size of data.
 *      > *first data point = days(x) *second data point = Prognostic_index(y)
 *        > once data is parsed and put into Vectors.
 *           > take [ y ] Log it for Z
 *     > find a_1  and  a_0 to find constants a and b
 *      use the linear interpolation equation
 *       -
 *      ** remember **
 *      a = e^a_0 and b = a_1
 *       -
 *      > now that you have your constants
 *         then you using N = ae^bx you can extrapolate data.
 *         > but first..
 *          Print:
 *          > data
 *          Data Summary:
 *                  > Mean
 *                  > Median
 *                  > Range
 *                  > Mode
 *                  > Standard Deviation
 *                  > the Equation best fit.
 *      Loop menu to to:
 *      > Extrapolate data ? || > Main Menu ^
 *          ->Prompt for days to extrapolate (x);
 *          ->output Prognostic data y;
 *       }
 *
 *
 *
 ************************************************************************************************************************/
#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

/*************************************************************************************************************************
 * Function Prototypes
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa
 ************************************************************************************************************************/
using namespace std;

int filesize(const string &filename);

int fileprocesing(ifstream file, vector<int> &vectorX, vector<int> &vectorY, int length);

double a_1(double sumxz, double sumx, double sumz, double sumxsqr, int length);

double a_nod(double avgx, double avgy, double a1);

double median(vector<int> vector);

double mean(const vector<int> &vector);

double mode(vector<int> vector);

double range(vector<int> vector);

double get_sum(const vector<int> &z, bool squared);

double get_sumz(const vector<int> &y);

double get_sum_tz(const vector<int> &x, const vector<int> &y);

double Stan_Dev(const vector<int> &vector, double mean);

/*************************************************************************************************************************
 * Function Name: Stan_Dev()
 * Purpose: this function tries to find the Standard Deviation of the residuals.
 * In Parameters: constant Vector, Double mean.
 * Out Parameters: the Standard Deviation
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa

 ************************************************************************************************************************/
double Stan_Dev(const vector<int> &vector, double mean) {
    double sumOfx = 0;
    for (int i : vector) {
        sumOfx += (double) pow((i - mean), 2);
    }
    return sqrt(sumOfx / (double) (vector.size() - 1));
}


/*************************************************************************************************************************
 * Function Name: get_sumz()
 * Purpose: this function gets the sum of the y vector and logs them to sum them up and find Z
 * In Parameters: const Vector<int> &y
 * Out Parameters: double sum of Z
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa

 ************************************************************************************************************************/
double get_sumz(const vector<int> &y) {
    double t = 0;
    double z;
    for (int x : y) {
        z = log((double) x);
        t += z;
    }
    return t;
}

/*************************************************************************************************************************
 * Function Name:get_sum
 * Purpose: to sum the contents of vector
 * In Parameters: vector &z, boolean sqared
 * Out Parameters: double t | f(t)
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa

 ************************************************************************************************************************/
double get_sum(const vector<int> &z, bool squared) {
    double t = 0;
    if (squared) {
        for (int x : z) {
            t += pow(x, 2);
        }
        return t;
    } else {
        for (int x : z) {
            t += x;
        }
    }
    return t;
    }

/*************************************************************************************************************************
 * Function Name:get_sum_tz()
 * Purpose: this function sums of the product of x and log(y)
 * In Parameters: &vectorX, & &vectorY
 * Out Parameters: Double Tx
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa
 ************************************************************************************************************************/
double get_sum_tz(const vector<int> &x, const vector<int> &y) {
    double tz = 0;
    double z;
    for (int j = 0; j < x.size(); j++) {
        z = log((double) y.at(j));
        tz += ((double) x.at(j) * z);
    }
    return tz;
}

/*************************************************************************************************************************
 * Function Name: test()
 * Purpose: this is the validate any input.
 * In Parameters: Istream &cin
 * Out Parameters: integer selection
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa

 ************************************************************************************************************************/
int test(istream &cin) {
    int selec;
    while ((cin >> selec).fail() || cin.peek() != '\n' || selec < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Input\n";
    }
    return selec;
}

double range(vector<int> vector) {
    sort(vector.begin(), vector.end());
    return vector.at(vector.size() - 1) - vector.at(0);
}

/*************************************************************************************************************************
 * Function Name: mode()
 * Purpose: to find the mode of the passed vector
 * In Parameters: vector
 * Out Parameters: the mode
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa

 ************************************************************************************************************************/
double mode(vector<int> vector) {
    sort(vector.begin(), vector.end());
    int max_count, res, count;
    max_count = 1;
    res = vector.at(0);
    count = 1;
    for (int i = 1; i < vector.size(); i++) {
        if (vector.at(i) == vector.at(i - 1)) {
            ++count;
        } else if (count > max_count) {
            max_count = count;
            res = vector.at(i - 1);
        }
        count = 1;
    }
    return res;
}

/*************************************************************************************************************************
 * Function Name: median()
 * Purpose: to find the median of the data.
 * In Parameters:
 * Out Parameters:
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa

 ************************************************************************************************************************/
double median(vector<int> vector) {
    sort(vector.begin(), vector.end());
    return vector.at(vector.size() / 2);
}

/*************************************************************************************************************************
 * Function Name: mean()
 * Purpose: to find the mean of what ever vector is passed
 * In Parameters: &vector
 * Out Parameters: double mean.
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa
 ************************************************************************************************************************/
double mean(const vector<int> &vector) {
    double mean = 0;
    for (int j : vector) {
        mean += j;
    }
    return mean / vector.size();
}

/*************************************************************************************************************************
 * Function Name: a_nod()
 * Purpose: to find a_nod from the linearized function.
 * ** Remember ** Z = anod + a1 * meanX
 * In Parameters: Double meanX, Double Meany, and a1
 * Out Parameters:
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa

 ************************************************************************************************************************/
double a_nod(double avgx, double avgy, double a1) {
    return avgy - a1 * avgx;
}

/*************************************************************************************************************************
 * Function Name: a_1()
 * Purpose: this is to determin A1 using the sum of square equation.
 * In Parameters: double sumxz, double sumx, double sumsqr, integer length.
 * Out Parameters:
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa

 ************************************************************************************************************************/
double a_1(double sumxz, double sumx, double sumz, double sumxsqr, int length) {
    return ((double) length * sumxz - (double) sumx * sumz) / (((double) length * sumxsqr) - (pow(sumx, 2)));
}

/*************************************************************************************************************************
 * Function Name: filesize()
 * Purpose: To retreive the size of the file and the size needed to allocated to my vectors.
 * In Parameters: File Address
 * Out Parameters: Integer Size, or -1 for failiur to open
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa

 ************************************************************************************************************************/
int filesize(const string &filename) {
    ifstream file;
    try {
        file = ifstream(filename);
    } catch (const ifstream::failure &e) {
        cout << "No such file." << endl;
        file.close();
        return -1;
    }
    string line;
    long i;
    int length;
//handfile exceptions

//skip first line
    getline(file, line);
//find number of lines.
    for (i = 0; getline(file, line); i++);
//determine amount of data
    length = i;
    file.close();
    return --length;

}

/*************************************************************************************************************************
 * Function Name: FileProcessing.
 * Purpose: processing the File to parse it and input the data into the vectors.
 *
 * In Parameters: file stream, VectorX and VectorY, Int length
 * Out Parameters: 0 for function success
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa

 ************************************************************************************************************************/
int fileprocesing(ifstream file, vector<int> &vectorX, vector<int> &vectorY, int length) {
    int i;
    string line;
    string x, y;
    // cout << "testercode *******************" << endl;
    for (i = 0; getline(file, line); i++) {
        istringstream iss(line);
        if (!(file >> x >> y)) {
            break;
        }
        // cout << " data\t" << x << "\t" << y << endl;
        vectorX.push_back(stoi(x));
        vectorY.push_back(stoi(y));
    }
    return 0;
}

/*************************************************************************************************************************
 * Function Name:       Main
 * Purpose:             Driver function of the program.
 *
 * In Parameters:       none
 * Out Parameters:      integer 0
 * Version:  1.0
 * Author: Meech Lamirande Kabuasa
 ************************************************************************************************************************/
int main() {

    vector<int> VectorX;
    vector<int> VectorY;
    int size;
    cout << "** Assignement 2 **" << endl;
    int selec;
    string filename;
    double meanx, meany,
            modey,
            mediany,
            stand_dev, sumx,
            sumxz, sumz,
            sumxsqr,
            a, b, a1, anod, rangey;

    //string Quickpath ="c:\\temp\\data.txt";

    do {
        cout << "MAIN MENU\n1. Exponential Fit\n2. Quit" << endl;
        selec = (int) test(cin);
        if (selec == 1) {
            cout << "Enter file name :" << endl;
            cin >> filename;
            size = filesize(filename);
            if (size < 0) {
                cout << "File doesn't exit" << endl;
            } else {
                cout << "" << endl;
                if (fileprocesing(ifstream(filename), VectorX, VectorY, size) == 0) {

                    //TODO 1. print the records 2. data summary(Mean, Median, Range,mode, standard Deviation) 3. the linear regression fit
                    cout << "i\tDays:\tPrognostic_index" << endl;
                    for (int i = 0; i < size; i++) {
                        cout<< "\t" << VectorX.at(i) << "\t" << VectorY.at(i) << endl;
                    }
                    //TODO Determining all my variables via function calls
                    sumx = get_sum(VectorX, false);
                    sumxsqr = get_sum(VectorX, true);
                    sumxz = get_sum_tz(VectorX, VectorY);
                    sumz = get_sumz(VectorY);
                    a1 = a_1(sumxz, sumx, sumz, sumxsqr, size);
                    meanx = mean(VectorX);
                    anod = a_nod(meanx, (double) (sumz / size), a1);

                    //TODO Data Summary
                    meany = mean(VectorY);
                    rangey = range(VectorY);
                    mediany = median(VectorY);
                    modey = mode(VectorY);
                    stand_dev = Stan_Dev(VectorX, meanx);
                    a = exp(anod);
                    b = a1;

                    //TODO Outputing the Data Summary
                    cout << "there are " << size << " records\n\nData Summary (prognostic_index)" << endl;
                    cout << "Mean: " << meany << endl;
                    cout << "Median: " << mediany << endl;
                    cout << "Range: " << rangey << endl;
                    cout << "Mode: " << modey << endl;
                    cout << "Standard Deviation: " << stand_dev << endl;


                    //TODO: THIS FINALLY WORKS!!!
                    cout << "Best Exponential Fit: Prognostic_index = " << a << " * exp^ " << b << "* days\n\n" << endl;

                    //TODO Now using the function constance a & b we can extrapolate data
                    int finddata;
                    while (selec != 3) {
                        cout << "SUB_MENU\n1. Extrapolate Data\n2. Back to main Menu" << endl;
                        selec = test(cin);
                        if (selec == 1) {
                            cout << "Please enter the days to extrapolate to: " << endl;
                            finddata = test(cin);
                            cout << " Prognostic_Index: " << a * exp(b * finddata) << endl;
                        } else if (selec == 2) {
                            selec = 3;
                        }
                    }
                }
            }
        }
        if (selec == 2)break;
    } while (true);
    return 0;//end of program.
}
/********************************* END OF PROGRAM ***********************************************************/