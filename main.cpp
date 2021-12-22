#include <iostream> 
#include <vector> 
#include <fstream> 
#include <string> 
#include <cstring> // for srting c-style
#include <cstdlib> /
#include <sstream> // for stringstream
#include <random> // for rd & mersenne
#include <Windows.h>

#include "my_functions.h" 

using namespace std;

int getRandom(int min, int max) {

    random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<> dist(min, max); // distribute results between min and max inclusive.

    return dist(gen);

}

int getIndex(int x, int y, int step) { //find links benwwen (n-1) layer and n layer

    return y * step + x;
}

int validation(string name, int value) {
    bool flag = true;
    int accuracy_or_test;
    while (flag) {
        cout << "Enter count of " << name << " (no more than " << value << "): "; cin >> accuracy_or_test;
        if (accuracy_or_test > value || accuracy_or_test - (int)accuracy_or_test > 0) {
            cout << "Bad data. Please, try again\n";
        }
        else {
            flag = false;
        }

    }
    return accuracy_or_test;
}

////////////////////////////////////////////////////////////////////////////////////// PNG to digit map
#define STB_IMAGE_IMPLEMENTATION    // activate realisation
#define STBI_ONLY_PNG   // only for PNG
#include "stb_image.h"
struct STB_png_mono_image
{

    // mono PNG image

    STB_png_mono_image() : m_width(), m_height(), m_comp(), m_data() {}
    ~STB_png_mono_image() { free_(); }
    // load from file
    bool load_from_file(const char* fname)
    {

        free_();
        // try to load image
        m_data = stbi_load(fname, &m_width, &m_height, &m_comp, 1);
        if (!m_data)
        {   // msg what go wrong
            std::cerr << stbi_failure_reason() << '\n';
            return false;
        }
        return true;
    }

    int width() const { return m_width; }
    int height() const { return m_height; }
    const unsigned char* data() const { return m_data; }

private:

    void free_()
    {
        if (m_data)
        {
            stbi_image_free(m_data);
            m_data = nullptr;
            m_width = m_height = m_comp = 0;
        }
    }

    int m_width;
    int m_height;
    int m_comp;
    unsigned char* m_data;
};
////////////////////////////////////////////////////////////////////////////////////// PNG to digit map


int main()
{
    const short int percepteronNeurons = 784;

    ////////////////////////////////////////////////////////////////////////////////////// initialization weights & layers
    vector <double> startLayer(percepteronNeurons); // 28 * 28 = 784
    vector <double> hiddenLayer(percepteronNeurons + 10);
    vector <double> finalLayer(10);

    vector <double> weights1((28 * 28) * (28 * 28 + 10)); // all links startLayer <-> hiddenLayer
    vector <double> weights2((28 * 28 + 10) * 10); // all links hiddenLayer <-> finalLayer

    ////////////////////////////////////////////////////////////////////////////////////// initialization weights & layers

    cout << "Create random weights or load \"The best weights\"? (create/load):  ";
    string weightsFlag;
    cin >> weightsFlag;
    cout << '\n';
    if (weightsFlag == "create")
    {
        ////////////////////////////////////////////////////////////////////////////////////// Creating random weight
        cout << "Creation new weights has been started... \n";

        for (int hl = 0; hl < 794; hl++) // generate random weights startLayer <-> hiddenLayer (weights1)
        {
            for (int sl = 0; sl < 784; sl++)
            {

                if (getRandom(1, 2) == 1)
                {
                    weights1[getIndex(hl, sl, 794)] = static_cast<double>(getRandom(1, 32767)) / 32767;
                }
                else
                {
                    weights1[getIndex(hl, sl, 794)] = -1 * static_cast<double>(getRandom(1, 32767)) / 32767;
                }
            }
        }


        for (int fl = 0; fl < 10; fl++) // generate random weights hiddenLayer <-> finalLayer (weights2)
        {
            for (int hl = 0; hl < 794; hl++)
            {
                if (getRandom(1, 2) == 1)
                {
                    weights2[getIndex(fl, hl, 10)] = static_cast<double>(getRandom(1, 32767)) / 32767;
                }
                else
                {
                    weights2[getIndex(fl, hl, 10)] = -1 * static_cast<double>(getRandom(1, 32767)) / 32767;
                }
            }
        }
        cout << "Creation new weights has been completed. \n\n";
        ////////////////////////////////////////////////////////////////////////////////////// Creating random weight
    }
    else
    {
        if (weightsFlag == "load")
        {
            ////////////////////////////////////////////////////////////////////////////////////// Load weights
            cout << "Loading of the weights has been started ... \n";
            string str;
            stringstream myString;


            ifstream file_weights1("my_files/file_weights1.txt");

            getline(file_weights1, str);

            myString << str;

            for (int hl = 0; hl < 794; hl++)
            {
                for (int sl = 0; sl < 784; sl++)
                {
                    myString >> weights1[getIndex(hl, sl, 794)];
                }
            }
            cout << "weights1 are loaded\n";
            myString.str(""); // clear the buffer
            myString.clear(); // clear all error's flag


            ifstream file_weights2("my_files/file_weights2.txt");

            getline(file_weights2, str);

            myString << str;

            for (int fl = 0; fl < 10; fl++)
            {
                for (int hl = 0; hl < 794; hl++)
                {
                    myString >> weights2[getIndex(fl, hl, 10)]; // write the weigths to one string
                }
            }
            cout << "weights2 are loaded\n\n";
            myString.str(""); // clear the buffer
            myString.clear(); // clear all error's flags
            ////////////////////////////////////////////////////////////////////////////////////// Load weights
        }
        else
        {
            cout << "Bad data!";
            return -1;
        }
    }



    double value_of_sufficient_contribution;

    cout << "Train or test? (train/test):  ";
    string humanChoise;
    cin >> humanChoise;
    cout << '\n';

    if (humanChoise == "train")
    {
        ////////////////////////////////////////////////////////////////////////////////////// TRAINING
 

        int count_of_training;
        count_of_training = validation("training", 60000);

        int count_of_test_accuracy;
        count_of_test_accuracy = validation("test accuracy", 10000);


        double total_accuracy;
        int count_right_answers{ 0 }, count_wrong_answers{ 1 }; // for accuracy
        int full_count_right_answers = 0; // for accuracy
        /////////////////////////// open file with image's name

        string name_png_file;
        string test_name_png_file;

        ifstream names_of_tests("train/readme2.txt"); // train <-> test //######################
        ifstream test_names_of_tests("test/readme2.txt");

        /////////////////////////// open file with image's name

        int count_of_succsess{ 0 }, count_of_errors{ 1 };
        double correction_step_from_final{ 0 }, correction_step_from_hidden{ 0 };


        for (int now_training_number = 0; now_training_number < count_of_training; now_training_number++) // the main cycle of training
        {

            /////////////////////////  get startLayer and right_answer
            getline(names_of_tests, name_png_file);

            name_png_file = "train/" + name_png_file; // train <-> test //######################

            if (name_png_file == "train/") // train <-> test //######################
            {
                name_png_file = "train/000000-num5.png"; // 5 <-> 7  &  train <-> test //######################
            }

            short right_answer = char_to_int(name_png_file[16]);

            auto name_png_file_c_style = name_png_file.c_str();


            for (char y = 0; y < 28; y++)
            {
                for (char x = 0; x < 28; x++)
                {
                    STB_png_mono_image img;
                    if (img.load_from_file(name_png_file_c_style))
                    {
                        unsigned char pix = img.data()[img.width() * y + x];

                        //cout << (int)pix;
                        startLayer[getIndex(x, y, 28)] = (static_cast<double>(pix) / 255);

                    }
                }
            }
            ///////////////////////// get startLayer and right_answer




            ///////////////////////// deleting past calculations in nodes
            for (int hl = 0; hl < 794; hl++)
            {
                hiddenLayer[hl] = 0;
            }

            for (int fl = 0; fl < 10; fl++)
            {
                finalLayer[fl] = 0;
            }
            ///////////////////////// deleting past calculations in nodes




            ///////////////////////// summing up the node values
            for (int hl = 0; hl < 794; hl++)
            {
                for (int sl = 0; sl < 784; sl++)
                {
                    hiddenLayer[hl] += weights1[getIndex(hl, sl, 794)] * startLayer[sl];
                }
                hiddenLayer[hl] = sigmoid_or_ReLU(hiddenLayer[hl] - 10.0);
            }


            for (int fl = 0; fl < 10; fl++)
            {
                for (int hl = 0; hl < 794; hl++)
                {
                    finalLayer[fl] += weights2[getIndex(fl, hl, 10)] * hiddenLayer[hl];
                }
                //cout << fl << ": " << finalLayer[fl] << "\n";
                //cout << finalLayer[fl] << "\n";
                finalLayer[fl] = sigmoid_or_ReLU(finalLayer[fl] - 10.0);
            }
            ///////////////////////// summing up the node values


            double ma{ 0 };
            int verdict;
            ///////////////////////// make a verdict
            for (int i = 0; i < 10; i++)
            {
                if (finalLayer[i] > ma)
                {
                    ma = finalLayer[i];
                    verdict = i;
                }
            }
            ///////////////////////// make a verdict


            if (verdict == right_answer)  count_of_succsess++;
            else count_of_errors++;

            if (count_of_errors + count_of_succsess == 2000)
            {
                cout << "train #: " << now_training_number + 2 << '/' << count_of_training << "  ";
                cout << "\tsuccess rate (last 2000): " << static_cast<double>(count_of_succsess) / (count_of_succsess + count_of_errors) << '\n';
                count_of_succsess = 0;
                count_of_errors = 0;

            }
            ///////////////////////// statistics output


            //#//#//#//#//#//#//#//#//#//#//#//#//  Back propagation  //#//#//#//#//#//#//#//#//#//#//#//#//
            value_of_sufficient_contribution = 0.7;

            correction_step_from_final = (1.0 - finalLayer[right_answer]) * (1.0 - finalLayer[right_answer]);


            for (int i = 0; i < 794; i++) // all neurons weights2
            {
                if (hiddenLayer[i] > value_of_sufficient_contribution) // if neuron from hidden layer makes great contribution
                {
                    // increasing the importance of link
                    weights2[getIndex(right_answer, i, 10)] += correction_step_from_final;

                }
            }


            for (int not_right_answer = 0; not_right_answer < 10; not_right_answer++)
            {
                if (not_right_answer != right_answer) // for all wrong answers in final layer
                {
                    correction_step_from_final = finalLayer[not_right_answer] * finalLayer[not_right_answer];

                    for (int i = 0; i < 794; i++) // all links weights2
                    {
                        if (hiddenLayer[i] > value_of_sufficient_contribution) // if link from hidden layer makes great contribution
                        {
                            // decreasing the importance of link with wrong answer
                            weights2[getIndex(not_right_answer, i, 10)] -= correction_step_from_final;

                        }
                    }
                }
            }
            //#//#//#//#//#//#//#//#//#//#//#//#//  Back propagation  //#//#//#//#//#//#//#//#//#//#//#//#//


        }  // Main cycle of training (the end)

        cout << '\n';
        cout << "The end of training, the beginning of accuracy calculations...." << '\n';

        //////////////////////////////////////////////////////// calculation accuracy

        for (int now_test_accuracy = 0; now_test_accuracy < count_of_test_accuracy; now_test_accuracy++) {

            getline(test_names_of_tests, test_name_png_file);

            test_name_png_file = "test/" + test_name_png_file;

            if (test_name_png_file == "test/")
            {
                test_name_png_file = "test/000000-num7.png";
            }

            short test_right_answer = char_to_int(test_name_png_file[15]);

            auto test_name_png_file_c_style = test_name_png_file.c_str();

            ///////////////////////// get startLayer
            for (char y = 0; y < 28; y++)
            {
                for (char x = 0; x < 28; x++)
                {
                    STB_png_mono_image img;
                    if (img.load_from_file(test_name_png_file_c_style))
                    {
                        unsigned char pix = img.data()[img.width() * y + x];

                        //cout << (int)pix;
                        startLayer[getIndex(x, y, 28)] = (static_cast<double>(pix) / 255);

                    }
                }
            }
            ///////////////////////// get startLayer


            ///////////////////////// delete past calculatons in nodes
            for (int hl = 0; hl < 794; hl++)
            {
                hiddenLayer[hl] = 0;
            }

            for (int fl = 0; fl < 10; fl++)
            {
                finalLayer[fl] = 0;
            }
            ///////////////////////// delete past calculatons in nodes


            ///////////////////////// summing up the node values
            for (int hl = 0; hl < 794; hl++)
            {
                for (int sl = 0; sl < 784; sl++)
                {
                    hiddenLayer[hl] += weights1[getIndex(hl, sl, 794)] * startLayer[sl];
                }
                hiddenLayer[hl] = sigmoid_or_ReLU(hiddenLayer[hl] - 10.0);
            }


            for (int fl = 0; fl < 10; fl++)
            {
                for (int hl = 0; hl < 794; hl++)
                {
                    finalLayer[fl] += weights2[getIndex(fl, hl, 10)] * hiddenLayer[hl];
                }

                finalLayer[fl] = sigmoid_or_ReLU(finalLayer[fl] - 10.0);
            }
            ///////////////////////// summing up the node values


            double ma{ 0 };
            int verdict;
            ///////////////////////// make the verdict
            for (int i = 0; i < 10; i++)
            {
                if (finalLayer[i] > ma)
                {
                    ma = finalLayer[i];
                    verdict = i;
                }
            }
            ///////////////////////// make the verdict

            if (test_right_answer = verdict) count_right_answers++;
            else count_wrong_answers++;

            if (count_right_answers + count_wrong_answers == 500) {

                full_count_right_answers += count_right_answers;
                cout << "test #: " << now_test_accuracy + 2 << '/' << count_of_test_accuracy << "  ";
                cout << "\tsuccess rate (last 500): " << static_cast<double>(count_right_answers) / (count_right_answers + count_wrong_answers) << '\n';
                cout << "total right answers: " << full_count_right_answers << "/" << now_test_accuracy + 2 << '\n';

                count_right_answers = 0;
                count_wrong_answers = 0;
            }
            total_accuracy = static_cast<double>(full_count_right_answers) / count_of_test_accuracy;



        }
        ///////////////////////////////////////////////////////////////////////////////// end of caclulations accuracy

        cout << '\n';




        ////////////////////////////////////////////////////////////////////////////////////// TRAINING



        ////////////////////////////////////////////////////////////////////////////////////// want to save trainig's results?
        string choise_of_update_weights;
        while (true)
        {
            cout << "\nUpdate \"the best weights\"? (y / n):  ";
            cin >> choise_of_update_weights;
            if (choise_of_update_weights == "y")
            {
                //////////////////////////////////////// update weights
                cout << "\n\n\nsave waights1. . . \n";
                ofstream file_weights1("my_files/file_weights1.txt");
                for (int hl = 0; hl < 794; hl++)
                {
                    for (int sl = 0; sl < 784; sl++)
                    {
                        file_weights1 << weights1[getIndex(hl, sl, 794)] << " ";
                    }
                }
                cout << "weights1 are saved\n\n";


                cout << "save waights2. . . \n";
                ofstream file_weights2("my_files/file_weights2.txt");
                for (int fl = 0; fl < 10; fl++)
                {
                    for (int hl = 0; hl < 794; hl++)
                    {
                        file_weights2 << weights2[getIndex(fl, hl, 10)] << " ";
                    }
                }
                cout << "weights2 are saved\n";
                cout << "\nGoodbye!";
                return 0;
                //////////////////////////////////////// update weights
            }
            else
            {
                if (choise_of_update_weights == "n")
                {
                    cout << "\nGoodbye!";
                    break;
                }
                else
                {
                    cout << "Bad data!\n";
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////////////////// want to save trainig's results?
    }
    else
    {
        if (humanChoise == "test")
        {
            while (true)
            {
                /////////////////////////// get path to file
                string path_to_png_file_from_human;
                cout << "Enter path to file.png: ";
                cin >> path_to_png_file_from_human;
                //std::cout << '\n';

                auto name_png_file_c_style = path_to_png_file_from_human.c_str();
                /////////////////////////// get path to file


                ///////////////////////// get startLayer
                for (char y = 0; y < 28; y++)
                {
                    for (char x = 0; x < 28; x++)
                    {
                        STB_png_mono_image img;
                        if (img.load_from_file(name_png_file_c_style))
                        {
                            unsigned char pix = img.data()[img.width() * y + x];

                            //cout << (int)pix;
                            startLayer[getIndex(x, y, 28)] = (static_cast<double>(pix) / 255);

                        }
                    }
                }
                ///////////////////////// get startLayer


                ///////////////////////// delete past calculatons in nodes
                for (int hl = 0; hl < 794; hl++)
                {
                    hiddenLayer[hl] = 0;
                }

                for (int fl = 0; fl < 10; fl++)
                {
                    finalLayer[fl] = 0;
                }
                ///////////////////////// delete past calculatons in nodes


                ///////////////////////// summing up the node values
                for (int hl = 0; hl < 794; hl++)
                {
                    for (int sl = 0; sl < 784; sl++)
                    {
                        hiddenLayer[hl] += weights1[getIndex(hl, sl, 794)] * startLayer[sl];
                    }
                    hiddenLayer[hl] = sigmoid_or_ReLU(hiddenLayer[hl] - 10.0);
                }


                for (int fl = 0; fl < 10; fl++)
                {
                    for (int hl = 0; hl < 794; hl++)
                    {
                        finalLayer[fl] += weights2[getIndex(fl, hl, 10)] * hiddenLayer[hl];
                    }

                    finalLayer[fl] = sigmoid_or_ReLU(finalLayer[fl] - 10.0);
                }
                ///////////////////////// summing up the node values


                double ma{ 0 };
                int verdict;
                ///////////////////////// make the verdict
                for (int i = 0; i < 10; i++)
                {
                    if (finalLayer[i] > ma)
                    {
                        ma = finalLayer[i];
                        verdict = i;
                    }
                }
                ///////////////////////// make the verdict

                cout << "============It IS: " << verdict << "\n\n";


            }
        }
        else
        {
            cout << "\nBad data!\n";
        }

    }


}


