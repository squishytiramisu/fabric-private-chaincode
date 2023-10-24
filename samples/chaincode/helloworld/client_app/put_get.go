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
            Name: "dummyEncrypt",
            Args: []string{"123123111"},
			Desc: " ELGAMAL TEST",
        },
        /*{
            Name: "PersonBorn",
            Args: []string{"123123111", "123123111", "Alice Monoro", "20021112"},
			Desc: "- invoke personBorn transaction to create valid person (Alice)",
        },
        {
            Name: "getPerson",
            Args: []string{"123123111"},
            Desc: "- invoke getPerson transaction to get person (Alice)",
        },
		{
            Name: "IssueHealthExamination",
            Args: []string{"123123111", "123123111", "20201010", "120", "80"},
			Desc: "- invoke healthRecord transaction to create health record for valid person (Alice) should succeed",
        },
        {
            Name: "getHealthExamination",
            Args: []string{"123123111"},
            Desc: "- invoke getHealthExamination transaction to get health examination of person (Alice)",
        },
		{
            Name: "IssueLifeInsurance",
            Args: []string{"123123111", "123123111", "20201010", "20301010","100", "200"},
			Desc: "- invoke lifeInsurance transaction to create life insurance for valid person (Alice) should succeed",
        },
        {
            Name: "getLifeInsurance",
            Args: []string{"123123111"},
            Desc: "- invoke getLifeInsurance transaction to get life insurance of person (Alice)",
        },
		{
            Name: "IssueWorkPermit",
            Args: []string{"123123111", "Alice Monoro", "20201010", "WorkPermitAgency"},
			Desc: "- invoke workPermit transaction to create work permit for valid person (Alice) should succeed now",
        },
        {
            Name: "getWorkPermit",
            Args: []string{"123123111"},
            Desc: "- invoke getWorkPermit transaction to get work permit of person (Alice)",
        },
        {
            Name: "getPerson",
            Args: []string{"123123111"},
            Desc: "- invoke getPerson transaction to get person (Alice)",
        },
        {
            Name: "getHealthExamination",
            Args: []string{"123123111"},
            Desc: "- invoke getHealthExamination transaction to get health examination of person (Alice)",
        },
        {
            Name: "getLifeInsurance",
            Args: []string{"123123111"},
            Desc: "- invoke getLifeInsurance transaction to get life insurance of person (Alice)",
        },
        {
            Name: "getWorkPermit",
            Args: []string{"123123111"},
            Desc: "- invoke getWorkPermit transaction to get work permit of person (Alice)",
        },*/
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