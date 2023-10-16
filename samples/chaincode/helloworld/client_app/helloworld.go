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
	network, err := utils.SetupNetwork(channelID)

	// Get FPC Contract
	contract := fpc.GetContract(network, ccID)

	// Invoke FPC Chaincode storeAsset
	logger.Infof("--> Invoke FPC Chaincode: store own JSON with TAJ")
	result, err := contract.SubmitTransaction("storeMyStruct", "123123123", "100")
	if err != nil {
		logger.Fatalf("Failed to Submit transaction: %v", err)
	}
	logger.Infof("--> Result: %s", string(result))

	// Evaluate FPC Chaincode retrieveAsset
	logger.Infof("--> Evaluate FPC Chaincode: stored TAJ check")
	result, err = contract.EvaluateTransaction("getMyStruct", "123123123")
	if err != nil {
		logger.Fatalf("Failed to Evaluate transaction: %v", err)
	}
	logger.Infof("--> Result: %s", string(result))


	// Store WRONG TAJ
	logger.Infof("--> Invoke FPC Chaincode: fake TAJ storing attempt")
	result_inv, err_inv := contract.SubmitTransaction("storeMyStruct", "ab123", "100")
	if err_inv != nil {
		logger.Fatalf("Failed to Submit transaction: %v", err_inv)
	}
	logger.Infof("--> Result: %s", string(result_inv))

	
}