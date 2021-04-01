// Code generated by counterfeiter. DO NOT EDIT.
package fakes

import (
	"sync"
)

type EncryptionContext struct {
	ConcealStub        func(string, []string) (string, error)
	concealMutex       sync.RWMutex
	concealArgsForCall []struct {
		arg1 string
		arg2 []string
	}
	concealReturns struct {
		result1 string
		result2 error
	}
	concealReturnsOnCall map[int]struct {
		result1 string
		result2 error
	}
	RevealStub        func([]byte) ([]byte, error)
	revealMutex       sync.RWMutex
	revealArgsForCall []struct {
		arg1 []byte
	}
	revealReturns struct {
		result1 []byte
		result2 error
	}
	revealReturnsOnCall map[int]struct {
		result1 []byte
		result2 error
	}
	invocations      map[string][][]interface{}
	invocationsMutex sync.RWMutex
}

func (fake *EncryptionContext) Conceal(arg1 string, arg2 []string) (string, error) {
	var arg2Copy []string
	if arg2 != nil {
		arg2Copy = make([]string, len(arg2))
		copy(arg2Copy, arg2)
	}
	fake.concealMutex.Lock()
	ret, specificReturn := fake.concealReturnsOnCall[len(fake.concealArgsForCall)]
	fake.concealArgsForCall = append(fake.concealArgsForCall, struct {
		arg1 string
		arg2 []string
	}{arg1, arg2Copy})
	stub := fake.ConcealStub
	fakeReturns := fake.concealReturns
	fake.recordInvocation("Conceal", []interface{}{arg1, arg2Copy})
	fake.concealMutex.Unlock()
	if stub != nil {
		return stub(arg1, arg2)
	}
	if specificReturn {
		return ret.result1, ret.result2
	}
	return fakeReturns.result1, fakeReturns.result2
}

func (fake *EncryptionContext) ConcealCallCount() int {
	fake.concealMutex.RLock()
	defer fake.concealMutex.RUnlock()
	return len(fake.concealArgsForCall)
}

func (fake *EncryptionContext) ConcealCalls(stub func(string, []string) (string, error)) {
	fake.concealMutex.Lock()
	defer fake.concealMutex.Unlock()
	fake.ConcealStub = stub
}

func (fake *EncryptionContext) ConcealArgsForCall(i int) (string, []string) {
	fake.concealMutex.RLock()
	defer fake.concealMutex.RUnlock()
	argsForCall := fake.concealArgsForCall[i]
	return argsForCall.arg1, argsForCall.arg2
}

func (fake *EncryptionContext) ConcealReturns(result1 string, result2 error) {
	fake.concealMutex.Lock()
	defer fake.concealMutex.Unlock()
	fake.ConcealStub = nil
	fake.concealReturns = struct {
		result1 string
		result2 error
	}{result1, result2}
}

func (fake *EncryptionContext) ConcealReturnsOnCall(i int, result1 string, result2 error) {
	fake.concealMutex.Lock()
	defer fake.concealMutex.Unlock()
	fake.ConcealStub = nil
	if fake.concealReturnsOnCall == nil {
		fake.concealReturnsOnCall = make(map[int]struct {
			result1 string
			result2 error
		})
	}
	fake.concealReturnsOnCall[i] = struct {
		result1 string
		result2 error
	}{result1, result2}
}

func (fake *EncryptionContext) Reveal(arg1 []byte) ([]byte, error) {
	var arg1Copy []byte
	if arg1 != nil {
		arg1Copy = make([]byte, len(arg1))
		copy(arg1Copy, arg1)
	}
	fake.revealMutex.Lock()
	ret, specificReturn := fake.revealReturnsOnCall[len(fake.revealArgsForCall)]
	fake.revealArgsForCall = append(fake.revealArgsForCall, struct {
		arg1 []byte
	}{arg1Copy})
	stub := fake.RevealStub
	fakeReturns := fake.revealReturns
	fake.recordInvocation("Reveal", []interface{}{arg1Copy})
	fake.revealMutex.Unlock()
	if stub != nil {
		return stub(arg1)
	}
	if specificReturn {
		return ret.result1, ret.result2
	}
	return fakeReturns.result1, fakeReturns.result2
}

func (fake *EncryptionContext) RevealCallCount() int {
	fake.revealMutex.RLock()
	defer fake.revealMutex.RUnlock()
	return len(fake.revealArgsForCall)
}

func (fake *EncryptionContext) RevealCalls(stub func([]byte) ([]byte, error)) {
	fake.revealMutex.Lock()
	defer fake.revealMutex.Unlock()
	fake.RevealStub = stub
}

func (fake *EncryptionContext) RevealArgsForCall(i int) []byte {
	fake.revealMutex.RLock()
	defer fake.revealMutex.RUnlock()
	argsForCall := fake.revealArgsForCall[i]
	return argsForCall.arg1
}

func (fake *EncryptionContext) RevealReturns(result1 []byte, result2 error) {
	fake.revealMutex.Lock()
	defer fake.revealMutex.Unlock()
	fake.RevealStub = nil
	fake.revealReturns = struct {
		result1 []byte
		result2 error
	}{result1, result2}
}

func (fake *EncryptionContext) RevealReturnsOnCall(i int, result1 []byte, result2 error) {
	fake.revealMutex.Lock()
	defer fake.revealMutex.Unlock()
	fake.RevealStub = nil
	if fake.revealReturnsOnCall == nil {
		fake.revealReturnsOnCall = make(map[int]struct {
			result1 []byte
			result2 error
		})
	}
	fake.revealReturnsOnCall[i] = struct {
		result1 []byte
		result2 error
	}{result1, result2}
}

func (fake *EncryptionContext) Invocations() map[string][][]interface{} {
	fake.invocationsMutex.RLock()
	defer fake.invocationsMutex.RUnlock()
	fake.concealMutex.RLock()
	defer fake.concealMutex.RUnlock()
	fake.revealMutex.RLock()
	defer fake.revealMutex.RUnlock()
	copiedInvocations := map[string][][]interface{}{}
	for key, value := range fake.invocations {
		copiedInvocations[key] = value
	}
	return copiedInvocations
}

func (fake *EncryptionContext) recordInvocation(key string, args []interface{}) {
	fake.invocationsMutex.Lock()
	defer fake.invocationsMutex.Unlock()
	if fake.invocations == nil {
		fake.invocations = map[string][][]interface{}{}
	}
	if fake.invocations[key] == nil {
		fake.invocations[key] = [][]interface{}{}
	}
	fake.invocations[key] = append(fake.invocations[key], args)
}
