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


    # Test Plan:
    # Try crate invalid person should fail
    # Try create valid person (Alice) should succeed
    # Try create valid person (Bob) should succeed
    # Try create work permit for invalid person should fail
    # Try create work permit for valid person (Alice) should fail, no life insurance
    # Try create life insurance for invalid person should fail
    # Try create life insurance for valid person (Alice) should fail, no health record
    # Try create health record for invalid person should fail
    # Try create health record for valid person (Alice) should succeed
    # Try create work permit for valid person (Alice) should fail again, no life insurance
    # Try create life insurance for valid person (Alice) should succeed
    # Try create work permit for valid person (Alice) should succeed now
    # Try create health record (unhealty) for valid person (Bob) should succeed
    # Try create work permit for valid person (Bob) should fail, no life insurance
    # Try create life insurance for valid person (Bob) should fail, not healthy
    # Try invalid person dies, should fail
    # Try valid person (Bob) dies, should succeed
    # Try create health record (healthy) for valid person (Bob) should fail, dead
    
    say "- interact with the FPC chaincode using our client app"
    export CC_ID
    export CHAN_ID
    go run client_app/helloworld.go

    return
    para
    # create invalid person should fail
    say "- invoke personBad transaction to create invalid person"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["PersonBorn","123123123","123123123","Johnatan Merquic","12"]}' --waitForEvent

    para
    # create valid person (Alice) should succeed
    say "- invoke personBorn transaction to create valid person (Alice)"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["PersonBorn","123123000","123123000","Alice Monoro","20001112"]}' --waitForEvent


    para
    # create valid person (Bob) should succeed
    say "- invoke personBorn transaction to create valid person (Bob)"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["PersonBorn","123123111","123123111","Bob Monoro","20021112"]}' --waitForEvent

    para
    # create work permit for invalid person should fail
    say "- invoke workPermit transaction to create work permit for invalid person"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueWorkPermit","123123123", "Alice Monoro", "20201010", "WorkPermitAgency"]}' --waitForEvent

    para
    # create work permit for valid person (Alice) should fail, no life insurance
    say "- invoke workPermit transaction to create work permit for valid person (Alice) should fail, no life insurance"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueWorkPermit","123123000", "Alice Monoro", "20201010", "WorkPermitAgency"]}' --waitForEvent

    para
    # create life insurance for invalid person should fail
    say "- invoke lifeInsurance transaction to create life insurance for invalid person should fail"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueLifeInsurance","123123123", "123123123", "20201010", "20301010","100", "200"]}' --waitForEvent

    para
    # create life insurance for valid person (Alice) should fail, no health record
    say "- invoke lifeInsurance transaction to create life insurance for valid person (Alice) should fail, no health record"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueLifeInsurance","123123000", "123123000", "20201010", "20301010","100", "200"]}' --waitForEvent

    para
    # create health record for invalid person should fail
    say "- invoke healthRecord transaction to create health record for invalid person should fail"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueHealthExamination","123123123", "123123123", "20201010", "120", "180"]}' --waitForEvent





    para
    # create health record for valid person (Alice) should succeed
    say "- invoke healthRecord transaction to create health record for valid person (Alice) should succeed"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueHealthExamination","123123000", "123123000", "20201010", "120", "80"]}' --waitForEvent

    para
    # create work permit for valid person (Alice) should fail again, no life insurance
    say "- invoke workPermit transaction to create work permit for valid person (Alice) should fail again, no life insurance"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueWorkPermit","123123000", "Alice Monoro", "20201010", "WorkPermitAgency"]}' --waitForEvent




    para
    # create life insurance for valid person (Alice) should succeed
    say "- invoke lifeInsurance transaction to create life insurance for valid person (Alice) should succeed"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueLifeInsurance","123123000", "123123000", "20201010", "20301010","100", "200"]}' --waitForEvent

    para
    # create work permit for valid person (Alice) should succeed now
    say "- invoke workPermit transaction to create work permit for valid person (Alice) should succeed now"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueWorkPermit","123123000", "Alice Monoro", "20201010", "WorkPermitAgency"]}' --waitForEvent

    para
    # create health record (unhealty) for valid person (Bob) should succeed
    say "- invoke healthRecord transaction to create health record (unhealty) for valid person (Bob) should succeed"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueHealthExamination","123123111", "123123111", "20201010", "210", "100"]}' --waitForEvent



    
    para
    # create work permit for valid person (Bob) should fail, no life insurance
    say "- invoke workPermit transaction to create work permit for valid person (Bob) should fail, no life insurance"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueWorkPermit","123123111", "Bob Monoro", "20201010", "WorkPermitAgency"]}' --waitForEvent

    para
    # create life insurance for valid person (Bob) should fail, not healthy
    say "- invoke workPermit transaction to create work permit for valid person (Bob) should fail, no life insurance"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueLifeInsurance","123123111", "123123111", "20201010", "20301010","100", "200"]}' --waitForEvent

    para
    # invalid person dies, should fail
    say "- invoke personDied transaction to invalid person dies, should fail"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["PersonDie","123123123"]}' --waitForEvent

    para
    # valid person (Bob) dies, should succeed
    say "- invoke personDied transaction to valid person (Bob) dies, should succeed"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["PersonDie","123123111"]}' --waitForEvent

    para
    # create health record (healthy) for valid person (Bob) should fail, dead
    say "- invoke healthRecord transaction to create health record (healthy) for valid person (Bob) should fail, dead"
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c '{"Args":["IssueHealthExamination","123123111", "123123111", "20201010", "120", "180"]}' --waitForEvent






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

