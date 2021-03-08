#include "Core/Core.hpp"
#include "Core/Logger/Logger.hpp"

#include "Domain/Algorithm/Algorithm.hpp"
#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"
#include "Domain/Algorithm/GradientDescentAlgorithm.hpp"
#include "Domain/Algorithm/ImplicitEnumerationAlgorithm.hpp"
#include "Domain/Algorithm/SimulatedAnnealingAlgorithm.hpp"
#include "Domain/Algorithm/TabuAlgorithm.hpp"

#include "Domain/Neighborhood/Neighborhood.hpp"
#include "Domain/Neighborhood/PickNDropNeighborhood.hpp"
#include "Domain/Neighborhood/SwapNeighborhood.hpp"
#include "Domain/Neighborhood/SweepNeighborhood.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"

#include <sstream>
#include <string>

// Todo
// output_path for solution
// details display
// timer on/off

void printUsage(const char* executableName);
void printAlgorithmUsage(const std::string& algorithm);

void processArgs(int argc, char** argv);

int main(int argc, char** argv)
{
    Yolo::Core::initialize();

    if (argc == 1)
    {
        printUsage(argv[0]);
    }

    if (argc == 2)
    {
        printAlgorithmUsage(argv[1]);
    }

    if (argc >= 3)
    {
        processArgs(argc, argv);
    }

    Yolo::Core::shutdown();
    return 0;
}

void printUsage(const char* executableName)
{
    std::stringstream ss;

    ss << "Usage\n\n";

    ss << "  <executable name> <mandatory arguments> [optional arguments]\n\n";

    ss << "  Mandatory arguments:\n\n";

    ss << "    nb_classes=<int> : Number of classes to dispatch vertices into.\n\n";

    ss << "    input_path=<string> : Path to the instance to be solved, either absolute or relative to the executable's directory.\n\n";

    ss << "  Optional arguments:\n\n";

    ss << "    algorithm=<string, default=gradient_descent> : Algorithm used to solve the instance, from the following list:\n\n";

    ss << "      explicit_enumeration\n";
    ss << "      implicit_enumeration\n";
    ss << "      gradient_descent\n";
    ss << "      tabu_list\n";
    ss << "      simulated_annealing\n\n";

    ss << "      Type '<executable name> algorithm=<string>' without any other arguments for detailed information on each algorithm's optional parameters.\n\n";

    ss << "  Example:\n\n";

    ss << "    " << executableName << " nb_classes=10 algorithm=explicit_enumeration input_path=some_instance.txt\n";

    YOLO_INFO(ss.str());
}

void printAlgorithmUsage(const std::string& algorithm)
{
    std::stringstream ss;

    const char* algorithms[] =
        {
            "explicit_enumeration",
            "implicit_enumeration",
            "gradient_descent",
            "tabu_list",
            "simulated_annealing",
            //
        };

    bool isAlgorithmValid = false;
    for (const char* algorithmIterator : algorithms)
    {
        if (algorithm == algorithmIterator)
        {
            isAlgorithmValid = true;
        }
    }

    if (!isAlgorithmValid)
    {
        ss << "Unknown algorithm.\n\n";

        ss << "  Available algorithms are:\n\n";

        ss << "  explicit_enumeration\n";
        ss << "  implicit_enumeration\n";
        ss << "  gradient_descent\n";
        ss << "  tabu_list\n";
        ss << "  simulated_annealing\n\n";

        ss << "  Type '<executable name> algorithm=<string>' without any other arguments for detailed information on each algorithm's optional parameters.\n";

        YOLO_INFO(ss.str());
        return;
    }

    ss << algorithm << " parameters \n\n";

    if (algorithm == "explicit_enumeration" || algorithm == "implicit_enumeration")
    {
        ss << "  None\n";
    }
    else if (algorithm == "gradient_descent" || algorithm == "tabu_list" || algorithm == "simulated_annealing")
    {
        ss << "  neighborhood=<string, default=pick_n_drop> : Neighborhood to explore, from the following list:\n\n";

        ss << "    swap : Explores solutions by swapping two elements each iteration.\n";

        ss << "    sweep : Explores solutions by sweeping a range of continuous vertices each iteration.\n";

        ss << "    pick_n_drop : Explores solutions by changing one vertex's class each iteration.\n\n";

        ss << "  max_iterations=<int, default=1000> : Maximum number of iterations.\n";

        if (algorithm == "gradient_descent")
        {
            ss << "  epsilon=<float, default=0.01> : Convergence threshold to stop the algorithm.\n";
        }
        else if (algorithm == "tabu_list")
        {
            ss << "  size=<int, default=100> : Tabu list's maximum size.\n";

            ss << "  store_all=<boolean, default=1> : Indicates if we store all the visited solutions or only the worse ones.\n";
        }
    }

    YOLO_INFO(ss.str());
}

std::vector<std::string> tokenizeString(std::string string, char limiter);

void processArgs(int argc, char** argv)
{
    constexpr const char* allowedArguments[] =
        {
            "nb_classes",
            "input_path",

            "algorithm",
            "neighborhood",
            "max_iterations",

            "epsilon",

            "size",
            "store_all",

            "starting_temperature"

            //
        };

    std::vector<std::string> arguments(static_cast<size_t>(argc) - 1);
    for (int i = 1; i < argc; ++i)
    {
        arguments[static_cast<size_t>(i) - 1] = std::string(argv[i]);
    }

    int nbClasses = 0;
    std::optional<Yolo::Graph> graphOpt;

    std::vector<std::vector<std::string>> tokenizedArguments;

    /* Search mandatory arguments and erase useless arguments */
    for (auto iterator = arguments.begin(); iterator < arguments.end(); ++iterator)
    {
        const auto tokens = tokenizeString(*iterator, '=');

        if (tokens.size() != 2)
        {
            std::stringstream ss;
            ss << "Removed " << *iterator << " argument.";

            YOLO_WARN(ss.str());
            arguments.erase(iterator);
            --iterator;

            continue;
        }

        bool invalidArgument = true;
        for (const auto& allowedArgument : allowedArguments)
        {
            if (strcmp((tokens[0]).c_str(), allowedArgument) == 0)
            {
                invalidArgument = false;
            }
        }

        if (invalidArgument)
        {
            std::stringstream ss;
            ss << "Removed " << *iterator << " argument.";

            YOLO_WARN(ss.str());
            arguments.erase(iterator);
            --iterator;

            continue;
        }

        if (tokens[0] == "input_path")
        {
            Yolo::GraphFileRepository graphRepository;
            graphOpt = graphRepository.load(tokens[1]);

            arguments.erase(iterator);
            --iterator;
        }
        else if (tokens[0] == "nb_classes")
        {
            nbClasses = std::stoi(tokens[1]);

            arguments.erase(iterator);
            --iterator;
        }
        else
        {
            tokenizedArguments.push_back(tokens);
        }
    }

    if (nbClasses <= 0)
    {
        YOLO_WARN("You're missing the mandatory argument nb_classes.");
    }

    if (!graphOpt)
    {
        YOLO_WARN("Could not load given instance. Perhaps you're missing the input_path argument?");
    }

    /* Search algorithm optional argument */
    std::string algorithm;
    for (const auto& argument : tokenizedArguments)
    {
        if (argument[0] == "algorithm")
        {
            algorithm = argument[1];
            break;
        }
    }

    std::optional<Yolo::Solution> output;

    if (algorithm == "explicit_enumeration")
    {
        Yolo::ExplicitEnumerationAlgorithm EE = Yolo::ExplicitEnumerationAlgorithm(*graphOpt, nbClasses);
        output = EE.solve();
    }
    else if (algorithm == "implicit_enumeration")
    {
        Yolo::ImplicitEnumerationAlgorithm IE = Yolo::ImplicitEnumerationAlgorithm(*graphOpt, nbClasses);
        output = IE.solve();
    }
    else if (algorithm == "gradient_descent")
    {
        Yolo::GradientDescentAlgorithm GD = Yolo::GradientDescentAlgorithm(*graphOpt, nbClasses);

        for (const auto& argument : tokenizedArguments)
        {
            if (argument[0] == "neighborhood")
            {
                if (argument[1] == "swap")
                {
                    GD.setNeighborhood(new Yolo::SwapNeighborhood());
                }
                else if (argument[1] == "sweep")
                {
                    GD.setNeighborhood(new Yolo::SweepNeighborhood());
                }
                else if (argument[1] == "pick_n_drop")
                {
                    GD.setNeighborhood(new Yolo::PickNDropNeighborhood());
                }
                else
                {
                    YOLO_INFO("Unknown neighborhood given. Running default one.");
                }
            }
            else if (argument[0] == "max_iterations")
            {
                GD.setMaxIterations(std::stoi(argument[1]));
            }
            else if (argument[0] == "epsilon")
            {
                GD.setEpsilon(std::stod(argument[1]));
            }
        }

        output = GD.solve();
    }
    else if (algorithm == "tabu_list")
    {
        Yolo::TabuAlgorithm TL = Yolo::TabuAlgorithm(*graphOpt, nbClasses);

        for (const auto& argument : tokenizedArguments)
        {
            if (argument[0] == "neighborhood")
            {
                if (argument[1] == "swap")
                {
                    TL.setNeighborhood(new Yolo::SwapNeighborhood());
                }
                else if (argument[1] == "sweep")
                {
                    TL.setNeighborhood(new Yolo::SweepNeighborhood());
                }
                else if (argument[1] == "pick_n_drop")
                {
                    TL.setNeighborhood(new Yolo::PickNDropNeighborhood());
                }
                else
                {
                    YOLO_INFO("Unknown neighborhood given. Running default one.");
                }
            }
            else if (argument[0] == "max_iterations")
            {
                TL.setMaxIterations(std::stoi(argument[1]));
            }
            else if (argument[0] == "size")
            {
                TL.setTabuListSize(std::stoi(argument[1]));
            }
            else if (argument[0] == "store_all")
            {
                TL.setStoreAll(std::stoi(argument[1]) != 0);
            }
        }

        output = TL.solve();
    }
    else if (algorithm == "simulated_annealing")
    {
        Yolo::SimulatedAnnealingAlgorithm SA = Yolo::SimulatedAnnealingAlgorithm(*graphOpt, nbClasses);

        for (const auto& argument : tokenizedArguments)
        {
            if (argument[0] == "neighborhood")
            {
                if (argument[1] == "swap")
                {
                    SA.setNeighborhood(new Yolo::SwapNeighborhood());
                }
                else if (argument[1] == "sweep")
                {
                    SA.setNeighborhood(new Yolo::SweepNeighborhood());
                }
                else if (argument[1] == "pick_n_drop")
                {
                    SA.setNeighborhood(new Yolo::PickNDropNeighborhood());
                }
                else
                {
                    YOLO_INFO("Unknown neighborhood given. Running default one.");
                }
            }
            else if (argument[0] == "max_iterations")
            {
                SA.setMaxIterations(std::stoi(argument[1]));
            }
            else if (argument[0] == "starting_temperature")
            {
                SA.setStartingTemperature(std::stod(argument[1]));
            }
        }

        output = SA.solve();
    }
    else
    {
        if (!algorithm.empty())
        {
            YOLO_INFO("Unknown algorithm. Launching default one.");
        }

        Yolo::GradientDescentAlgorithm defaultAlgorithm = Yolo::GradientDescentAlgorithm(*graphOpt, nbClasses);
        output = defaultAlgorithm.solve();
    }

    if (output)
    {
        std::stringstream ss1;
        ss1 << "Solution = " << output->toString();
        YOLO_INFO(ss1.str());

        std::stringstream ss2;
        ss2 << "Cost = " << graphOpt->getSolutionCost(*output);
        YOLO_INFO(ss2.str());
    }
    else
    {
        YOLO_INFO("No solution found for this instance.");
    }
}

std::vector<std::string> tokenizeString(std::string string, char limiter)
{
    std::vector<std::string> elements;

    size_t position = 0;
    size_t previousPosition = 0;

    while (position != std::string::npos)
    {
        position = string.find_first_of(limiter, previousPosition);

        elements.push_back(string.substr(previousPosition, position - previousPosition));

        previousPosition = position + 1;
    }

    return elements;
}
