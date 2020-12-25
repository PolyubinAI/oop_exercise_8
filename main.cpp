
//Полюбин Арсений, М80-206Б-19, Вариант 18: Трапеция, квадрат прямоугольник

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include "Figure.hpp"
#include "Factory.hpp"
#include "pub_sub.hpp"
using namespace std;
using SCALAR_TYPE = double;
const string FOLDER = "";
ofstream file;
PubSub <vector<shared_ptr<Figure<SCALAR_TYPE>>>> pubsub;
int part = 1;
string genFileName(size_t n) {
    string res = to_string(n)+".txt";
    return res;
}

void ThreadFunc() {
    using functionType = function<void(shared_ptr<Figure<SCALAR_TYPE>> fig)>;
    //Обработчик для вывода в cout
    functionType PrintToStdout = [](const shared_ptr<Figure<SCALAR_TYPE>> fig) {
        (*fig).print();
    };
    //Обработчик для вывода в ofstream
    functionType WriteToFile = [](const shared_ptr<Figure<SCALAR_TYPE>> fig) {
        (*fig).print(file);
    };
    vector<functionType> funcs({PrintToStdout, WriteToFile});
    bool awake = true;
    while (awake) {
        if (!pubsub.Empty()) {
            vector<shared_ptr<Figure<SCALAR_TYPE>>> message = pubsub.Front();
            if (message.empty()) {
                awake = false;
            }
            string filename = (FOLDER + genFileName(part));
            file = ofstream(filename);
            while (!message.empty()) {
                shared_ptr<Figure<SCALAR_TYPE>> figPtr = message.back();
                message.pop_back();
                for (auto func : funcs) {
                    func(figPtr);
                }
            }
            file.close();
            ++part;
            pubsub.Pop();
        }
    }
}
int main(int argc, char** argv)
{
    Factory<SCALAR_TYPE> fact;
    size_t bufferSize;
    if(argc != 2){
        cout << "Wrong count of arguments!" << "\n";
        return -1;
    }
    try {
        bufferSize = stoi(string(argv[1]));
    }
    catch (exception & ex) {
        cout << ex.what() << "\n";
        return -1;
    }
    if(bufferSize <= 0){
        cout << "Wrong size!" << "\n";
        return -1;
    }
    thread myThread(ThreadFunc);
    vector<shared_ptr<Figure<SCALAR_TYPE>>> figures;
    string type;
    std::cout << "Введите тип фигуры square/rectangle/trapeze\n";
    while (cin >> type) {
        figures.push_back(fact.create_from_type(cin, type));
        if (figures.size() == bufferSize) {
            pubsub.Push(figures);
            bool threadWorkDone = false;
            while (!threadWorkDone) {
                if (pubsub.Empty()){
                    threadWorkDone = true;
                }
            }
            figures.clear();
        }
        std::cout << "Введите тип фигуры square/rectangle/trapeze\n";
    }
    pubsub.Push(figures);
    figures.clear();
    myThread.join();
    return 0;
}