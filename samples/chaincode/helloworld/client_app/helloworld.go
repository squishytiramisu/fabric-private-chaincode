package main

import (
	"os"

	fpc "github.com/hyperledger/fabric-private-chaincode/client_sdk/go/pkg/gateway"
	"github.com/hyperledger/fabric-private-chaincode/integration/client_sdk/go/utils"
	"github.com/hyperledger/fabric/common/flogging"
)

var logger = flogging.MustGetLogger("helloworld")

func main() {
	ccID := os.Getenv("CC_ID")
	logger.Infof("Use Chaincode ID: %v", ccID)

	channelID := os.Getenv("CHAN_ID")
	logger.Infof("Use channel: %v", channelID)

	// get network
	network, _ := utils.SetupNetwork(channelID)

	// Get FPC Contract
	contract := fpc.GetContract(network, ccID)


	transactions := []struct {
        Name string
        Args []string
		Desc string
    }{
        {
            Name: "PersonBorn",
            Args: []string{"123123123", "123123123", "Johnatan Merquic", "12"},
			Desc: "- invoke personBad transaction to create invalid person",
        },
        {
            Name: "PersonBorn",
            Args: []string{"123123000", "123123000", "Alice Monoro", "20001112"},
			Desc: "- invoke personBorn transaction to create valid person (Alice)",
        },
        {
            Name: "PersonBorn",
            Args: []string{"123123111", "123123111", "Bob Monoro", "20021112"},
			Desc: "- invoke personBorn transaction to create valid person (Bob)",
        },
		{
            Name: "IssueWorkPermit",
            Args: []string{"123123123", "Alice Monoro", "20201010", "WorkPermitAgency"},
			Desc: "- invoke workPermit transaction to create work permit for invalid person",
        },
		{
            Name: "IssueWorkPermit",
            Args: []string{"123123000", "Alice Monoro", "20201010", "WorkPermitAgency"},
			Desc: "- invoke workPermit transaction to create work permit for valid person (Alice) should fail, no life insurance",
		},
		{
            Name: "IssueLifeInsurance",
            Args: []string{"123123123", "123123123", "20201010", "20301010","100", "200"},
			Desc: "- invoke lifeInsurance transaction to create life insurance for invalid person should fail",
		},
		{
            Name: "IssueLifeInsurance",
            Args: []string{"123123000", "123123000", "20201010", "20301010","100", "200"},
			Desc: "- invoke lifeInsurance transaction to create life insurance for valid person (Alice) should fail, no health record",
		},
		{
            Name: "IssueHealthExamination",
            Args: []string{"123123123", "123123123", "20201010", "120", "80"},
			Desc: "- invoke healthRecord transaction to create health record for invalid person should fail",
		},
		{
            Name: "IssueHealthExamination",
            Args: []string{"123123000", "123123000", "20201010", "120", "80"},
			Desc: "- invoke healthRecord transaction to create health record for valid person (Alice) should succeed",
        },
		{
            Name: "IssueWorkPermit",
            Args: []string{"123123000", "Alice Monoro", "20201010", "WorkPermitAgency"},
			Desc: "- invoke workPermit transaction to create work permit for valid person (Alice) should fail again, no life insurance",
        },
		{
            Name: "IssueLifeInsurance",
            Args: []string{"123123000", "123123000", "20201010", "20301010","100", "200"},
			Desc: "- invoke lifeInsurance transaction to create life insurance for valid person (Alice) should succeed",
        },
		{
            Name: "IssueWorkPermit",
            Args: []string{"123123000", "Alice Monoro", "20201010", "WorkPermitAgency"},
			Desc: "- invoke workPermit transaction to create work permit for valid person (Alice) should succeed now",
        },
		{
            Name: "IssueHealthExamination",
            Args: []string{"123123111", "123123111", "20201010", "210", "100"},
			Desc: "- invoke healthRecord transaction to create health record (unhealty) for valid person (Bob) should succeed",
        },
		{
            Name: "IssueWorkPermit",
            Args: []string{"123123111", "Bob Monoro", "20201010", "WorkPermitAgency"},
			Desc: "- invoke workPermit transaction to create work permit for valid person (Bob) should fail, no life insurance",
        },
		{
            Name: "IssueLifeInsurance",
            Args: []string{"123123111", "123123111", "20201010", "20301010","100", "200"},
			Desc: "- invoke workPermit transaction to create work permit for valid person (Bob) should fail, no life insurance",
        },
		{
            Name: "PersonDie",
            Args: []string{"123123123"},
			Desc: "- invoke personDied transaction to invalid person dies, should fail",
        },
		{
            Name: "PersonDie",
            Args: []string{"123123111"},
			Desc: "- invoke personDied transaction to valid person (Bob) dies, should succeed",
        },
		{
            Name: "IssueHealthExamination",
            Args: []string{"123123111", "123123111", "20201010", "120", "180"},
			Desc: "- invoke healthRecord transaction to create health record (healthy) for valid person (Bob) should fail, dead",
        },

    }

    // Iterate through the transactions and invoke them
    for _, tx := range transactions {
        result, err1 := contract.SubmitTransaction(tx.Name, tx.Args...)
		logger.Infof("Invoking %s %s ", tx.Name, tx.Desc)
        if err1 != nil {
            // Handle the error
           logger.Fatalf("Failed to invoke transaction: " + err1.Error())
        }

        // Process the result
        logger.Infof("Result of %s: %s\n", tx.Name, string(result))
    }


	
}