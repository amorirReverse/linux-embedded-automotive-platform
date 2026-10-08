// tests/test_configuration.cpp

#include "Configuration.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

/**
 * @brief Entry point for the Configuration tests.
 *
 * @return Zero if all tests pass, non-zero otherwise.
 */
int main()
{
    const std::filesystem::path tempDirectory =
        std::filesystem::temp_directory_path();

    Configuration configuration("config/platform.conf");

    if (configuration.getSimulationPeriodMs() != 250)
    {
        std::cerr << "Unexpected simulation period: "
                  << configuration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    const std::filesystem::path invalidConfigPath =
        tempDirectory / "invalid_platform.conf";

    {
        std::ofstream configFile(invalidConfigPath);
        configFile << "simulation_period_ms=invalid\n";
    }

    Configuration invalidConfiguration(invalidConfigPath);

    if (invalidConfiguration.getSimulationPeriodMs() != 100)
    {
        std::cerr << "Invalid configuration did not use default value: "
                  << invalidConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(invalidConfigPath);

    const std::filesystem::path outOfRangeConfigPath =
        tempDirectory / "out_of_range_platform.conf";

    {
        std::ofstream configFile(outOfRangeConfigPath);
        configFile << "simulation_period_ms=999999999999999999999999\n";
    }

    Configuration outOfRangeConfiguration(outOfRangeConfigPath);

    if (outOfRangeConfiguration.getSimulationPeriodMs() != 100)
    {
        std::cerr << "Out of range configuration did not use default value: "
                  << outOfRangeConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(outOfRangeConfigPath);

    const std::filesystem::path zeroConfigPath =
        tempDirectory / "zero_platform.conf";

    {
        std::ofstream configFile(zeroConfigPath);
        configFile << "simulation_period_ms=0\n";
    }

    Configuration zeroConfiguration(zeroConfigPath);

    if (zeroConfiguration.getSimulationPeriodMs() != 100)
    {
        std::cerr << "Zero simulation period did not use default value: "
                  << zeroConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(zeroConfigPath);

    const std::filesystem::path negativeConfigPath =
        tempDirectory / "negative_platform.conf";

    {
        std::ofstream configFile(negativeConfigPath);
        configFile << "simulation_period_ms=-50\n";
    }

    Configuration negativeConfiguration(negativeConfigPath);

    if (negativeConfiguration.getSimulationPeriodMs() != 100)
    {
        std::cerr << "Negative simulation period did not use default value: "
                  << negativeConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(negativeConfigPath);

    const std::filesystem::path validConfigPath =
        tempDirectory / "valid_platform.conf";

    {
        std::ofstream configFile(validConfigPath);
        configFile << "simulation_period_ms=500\n";
    }

    Configuration validConfiguration(validConfigPath);

    if (validConfiguration.getSimulationPeriodMs() != 500)
    {
        std::cerr << "Valid configuration value was not loaded correctly: "
                  << validConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(validConfigPath);
    const std::filesystem::path spacedConfigPath =
        tempDirectory / "spaced_platform.conf";

    {
        std::ofstream configFile(spacedConfigPath);
        configFile << "   simulation_period_ms = 750\n";
    }

    Configuration spacedConfiguration(spacedConfigPath);

    if (spacedConfiguration.getSimulationPeriodMs() != 750)
    {
        std::cerr << "Configuration with spaces was not loaded correctly: "
                  << spacedConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(spacedConfigPath);

    const std::filesystem::path whitespaceConfigPath =
        tempDirectory / "whitespace_platform.conf";

    {
        std::ofstream configFile(whitespaceConfigPath);
        configFile << "   \n";
        configFile << "\t\t\n";
        configFile << "simulation_period_ms=600\n";
    }

    Configuration whitespaceConfiguration(whitespaceConfigPath);

    if (whitespaceConfiguration.getSimulationPeriodMs() != 600)
    {
        std::cerr << "Configuration with whitespace-only lines was not loaded correctly: "
                  << whitespaceConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(whitespaceConfigPath);

    const std::filesystem::path paddedConfigPath =
        tempDirectory / "padded_platform.conf";

    {
        std::ofstream configFile(paddedConfigPath);
        configFile << "  simulation_period_ms   =   850  \n";
    }

    Configuration paddedConfiguration(paddedConfigPath);

    if (paddedConfiguration.getSimulationPeriodMs() != 850)
    {
        std::cerr << "Configuration with padded key and value was not loaded correctly: "
                  << paddedConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(paddedConfigPath);

    const std::filesystem::path indentedCommentConfigPath =
        tempDirectory / "indented_comment_platform.conf";

    {
        std::ofstream configFile(indentedCommentConfigPath);
        configFile << "   # This line is a comment\n";
        configFile << "\t# This line is also a comment\n";
        configFile << "simulation_period_ms=900\n";
    }

    Configuration indentedCommentConfiguration(indentedCommentConfigPath);

    if (indentedCommentConfiguration.getSimulationPeriodMs() != 900)
    {
        std::cerr << "Indented comments were not ignored correctly: "
                  << indentedCommentConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(indentedCommentConfigPath);

    const std::filesystem::path missingConfigPath =
        tempDirectory / "missing_platform.conf";

    Configuration missingConfiguration(missingConfigPath);

    if (missingConfiguration.getSimulationPeriodMs() != 100)
    {
        std::cerr << "Missing configuration did not use default value: "
                  << missingConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    const std::filesystem::path missingKeyConfigPath =
        tempDirectory / "missing_key_platform.conf";

    {
        std::ofstream configFile(missingKeyConfigPath);
        configFile << "# Configuration without simulation period\n";
    }

    Configuration missingKeyConfiguration(missingKeyConfigPath);

    if (missingKeyConfiguration.getSimulationPeriodMs() != 100)
    {
        std::cerr << "Missing key configuration did not use default value: "
                  << missingKeyConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(missingKeyConfigPath);

    const std::filesystem::path malformedConfigPath =
        tempDirectory / "malformed_platform.conf";

    {
        std::ofstream configFile(malformedConfigPath);
        configFile << "simulation_period_ms\n";
    }

    Configuration malformedConfiguration(malformedConfigPath);

    if (malformedConfiguration.getSimulationPeriodMs() != 100)
    {
        std::cerr << "Malformed configuration did not use default value: "
                  << malformedConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(malformedConfigPath);

    const std::filesystem::path missingValueConfigPath =
        tempDirectory / "missing_value_platform.conf";

    {
        std::ofstream configFile(missingValueConfigPath);
        configFile << "simulation_period_ms=\n";
    }

    Configuration missingValueConfiguration(missingValueConfigPath);

    if (missingValueConfiguration.getSimulationPeriodMs() != 100)
    {
        std::cerr << "Missing configuration value did not use default value: "
                  << missingValueConfiguration.getSimulationPeriodMs()
                  << " ms."
                  << std::endl;
        return 1;
    }

    std::filesystem::remove(missingValueConfigPath);

    std::cout << "Configuration tests passed."
              << std::endl;

    return 0;
}