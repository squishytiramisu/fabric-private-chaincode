#!/usr/bin/env bash

if [[ -z "${FPC_PATH}" ]]; then
  echo "Error: FPC_PATH not set"; exit 1
fi

FABRIC_CFG_PATH="${FPC_PATH}/integration/config"
FABRIC_SCRIPTDIR="${FPC_PATH}/fabric/bin/"

. ${FABRIC_SCRIPTDIR}/lib/common_utils.sh
. ${FABRIC_SCRIPTDIR}/lib/common_ledger.sh

# this is the path points to FPC chaincode binary
CC_PATH=${FPC_PATH}/samples/chaincode/helloworld/_build/lib/

CC_ID=helloworld_test
CC_VER="$(cat ${CC_PATH}/mrenclave)"
CC_EP="OR('SampleOrg.member')"
CC_SEQ="1"

run_test() {
    # install helloworld chaincode
    # input:  CC_ID:chaincode name; CC_VER:chaincode version;
    #         CC_PATH:path to build artifacts
    say "- install helloworld chaincode"
    PKG=/tmp/${CC_ID}.tar.gz
    ${PEER_CMD} lifecycle chaincode package --lang fpc-c --label ${CC_ID} --path ${CC_PATH} ${PKG}
    ${PEER_CMD} lifecycle chaincode install ${PKG}

    PKG_ID=$(${PEER_CMD} lifecycle chaincode queryinstalled | awk "/Package ID: ${CC_ID}/{print}" | sed -n 's/^Package ID: //; s/, Label:.*$//;p')

    ${PEER_CMD} lifecycle chaincode approveformyorg -o ${ORDERER_ADDR} -C ${CHAN_ID} --package-id ${PKG_ID} --name ${CC_ID} --version ${CC_VER} --sequence ${CC_SEQ} --signature-policy ${CC_EP}
    ${PEER_CMD} lifecycle chaincode checkcommitreadiness -C ${CHAN_ID} --name ${CC_ID} --version ${CC_VER} --sequence ${CC_SEQ} --signature-policy ${CC_EP}
    ${PEER_CMD} lifecycle chaincode commit -o ${ORDERER_ADDR} -C ${CHAN_ID} --name ${CC_ID} --version ${CC_VER} --sequence ${CC_SEQ} --signature-policy ${CC_EP}

    # create an FPC chaincode enclave
    ${PEER_CMD} lifecycle chaincode initEnclave -o ${ORDERER_ADDR} --peerAddresses "localhost:7051" --name ${CC_ID}

    # store the value of 100 in asset1
    say "- invoke storeAsset transaction to store value 100 in asset1"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["storeAsset","asset1","100"]}' --waitForEvent

    # retrieve current value for "asset1";  should be 100;
    say "- invoke retrieveAsset transaction to retrieve current value of asset1"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["retrieveAsset","asset1"]}' --waitForEvent

    say "- invoke query with retrieveAsset transaction to retrieve current value of asset1"
    ${PEER_CMD} chaincode query  -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["retrieveAsset","asset1"]}'

    


    say "- interact with the FPC chaincode using our client app"
    export CC_ID
    export CHAN_ID
    go run client_app/helloworld.go

}

trap ledger_shutdown EXIT

say "Setup ledger ..."
ledger_init

para
say "Run helloworld test ..."
run_test

para
say "Shutdown ledger ..."
ledger_shutdown

yell "Helloworld test PASSED"

exit 0

