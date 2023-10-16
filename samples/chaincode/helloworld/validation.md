### Validations with the chain

Objects:

Person: ID TAJ NAME BD SEX
Health examination: TAJ SIS DIA DATE
Life insurance: ID TAJ FROM TO COST PAYMENT SHOULD_PAY
Work Permit: ID NAME FROM

Events:
Person: Born, Die
Health examination: Issue
Life Insurance: Issue
Work Permit: Issue, Revoke

Validations:

Born: Valid data
Die: Exsisting person
HealthIssue: Existing person, sis dis in range, data in past
LifeInsurance: Existing person, Existing health exam in 6 months, sis dis in range, from is future, to is future++
work permit: existing person, existing life insurance, 