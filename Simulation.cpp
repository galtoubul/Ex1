#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "Simulation.h"
using std::vector;
using std::tuple;
using std::get;
using std::ofstream;
using std::string;
#define BEFORE_FIRST_PORT -1
//ShipPlan* shipPlan;
//ShipRoute* shipRoute;
Port* currPort;
//
//
//void travel(string shipPlanFileName, string shipRouteFileName){
//    initSimulation(shipPlanFileName, shipRouteFileName);
//    currPortI = -1;
//    for (Port port : shipRoute->getPortList()){
//        currPortI++;
//        currPort = &port;
//        input_full_path_and_file_name = ;// find the proper file using extPortIdFromFileName
//        getInstructionsForCargo(input_full_path_and_file_name, output_full_path_and_file_name); //figure out what they want the output file to be
//    }
//}

void Simulator::startTravel (Algorithm* algorithm, const string& travelName){
    currPortIndex = BEFORE_FIRST_PORT;
    for (Port* port : this->shipRoute.getPortList()){

        std::cout << "now in port" << port->getPortId() << std:: endl;

        currPortIndex++;
        currPort = port;
        algorithm -> setCurrPort(port);
        string inputFileName, outputFileName;
        bool isFinalPort = (size_t)currPortIndex == this->shipRoute.getPortList().size()-1;
        getPortFilesName(inputFileName, outputFileName, currPort->getPortId(), currPortIndex, travelName, isFinalPort);
        if (isFinalPort){
            std::cout << "Simulator -- There are "<< algorithm->getCurrPort()->getContainersToUnload().size() << " containers to unload to port" << algorithm->getCurrPort()->getPortId() << " :" << std:: endl;
            for (Container* container : algorithm->getCurrPort()->getContainersToUnload())
                std::cout << *container << std:: endl;
            algorithm->getInstructionsForCargo("finalPort", outputFileName);
            continue; //==break;
        }
        vector<Container*> containersAwaitingAtPort;
        readContainersAwaitingAtPort(inputFileName, containersAwaitingAtPort);

        std::cout << "Simulator -- There are "<< containersAwaitingAtPort.size() << " containers awaiting at port" << algorithm->getCurrPort()->getPortId() << " :" << std:: endl;
        for (Container* container : containersAwaitingAtPort)
            std::cout << *container << std:: endl;
        std::cout << "Simulator -- There are "<< algorithm->getCurrPort()->getContainersToUnload().size() << " containers to unload to port" << algorithm->getCurrPort()->getPortId() << " :" << std:: endl;
        for (Container* container : algorithm->getCurrPort()->getContainersToUnload())
            std::cout << *container << std:: endl;
        algorithm->getInstructionsForCargo(inputFileName, outputFileName);
    }
}

void Simulator::initSimulation (int algorithmNum, int travelNum){
    string travelName = "Travel" + std::to_string(travelNum);
    std::cout << "Starting travel: "<< travelName << std:: endl;

    string shipPlanPath = travelName +  std::string(1, std::filesystem::path::preferred_separator) + "Ship Plan.txt";
    string shipRoutePath = travelName + std::string(1, std::filesystem::path::preferred_separator) + "Route.txt";
    this->getInput(shipPlanPath, shipRoutePath);

    Algorithm* algorithm = Algorithm::createAlgorithm(algorithmNum);
    algorithm->getInput(shipPlanPath, shipRoutePath);

    WeightBalanceCalculator calculator;
    algorithm->setWeightBalanceCalculator(calculator);

    startTravel(algorithm, travelName);
    std::cout << "End of travel: "<< travelName << std:: endl;
}

void Simulator::getInput(const string& shipPlanFileName, const string& shipRouteFileName){
    readShipPlan(this->shipPlan, shipPlanFileName);
    readShipRoute(this->shipRoute, shipRouteFileName);
}

const ShipPlan& Simulator::getShipPlan () const{
    return this->shipPlan;
}

const ShipRoute& Simulator::getShipRoute() const{
    return this->shipRoute;
}

std::ostream& operator<<(std::ostream& out, const Simulator& simulator){
    out << "Ship Plan: " << '\n';
    simulator.getShipPlan().printShipPlan();
    out << simulator.getShipRoute() << '\n';
    return out;
}

/*Port* Simulator::findPortFromId(const string& portId){
    for (Port* port : this->shipRoute.getPortList()) {
        if (port->getPortId() == portId)
            return port;
    }
    return nullptr; //won't reach there
}*/