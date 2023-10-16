//go:build exclude
// +build exclude

// TODO remove build tag

/*
Copyright IBM Corp. All Rights Reserved.
Copyright 2020 Intel Corporation

SPDX-License-Identifier: Apache-2.0
*/

package main

import (
	"os"

	"github.com/hyperledger/fabric-chaincode-go/shim"
	"github.com/hyperledger/fabric-contract-api-go/contractapi"
	fpc "github.com/hyperledger/fabric-private-chaincode/ecc_go/chaincode"
	"github.com/hyperledger/fabric-samples/asset-transfer-basic/chaincode-go/chaincode"
)

func main() {

	ccid := os.Getenv("CHAINCODE_PKG_ID")
	addr := os.Getenv("CHAINCODE_SERVER_ADDRESS")

	// create chaincode
	// TODO add chaincode

	assetChaincode, _ := contractapi.NewChaincode(&chaincode.SmartContract{})
	chaincode := fpc.NewPrivateChaincode(assetChaincode)

	// start chaincode as a service
	server := &shim.ChaincodeServer{
		CCID:    ccid,
		Address: addr,
		CC:      chaincode,
		TLSProps: shim.TLSProperties{
			Disabled: true, // just for testing good enough
		},
	}

	if err := server.Start(); err != nil {
		panic(err)
	}
}
