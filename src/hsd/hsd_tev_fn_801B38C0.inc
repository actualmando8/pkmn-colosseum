    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmpwi r3, 0x5
    stw r0, 0x14(r1)
    beq @L_801B3944
    bge @L_801B3900
    cmpwi r3, 0x2
    beq @L_801B392C
    bge @L_801B38F4
    cmpwi r3, 0x0
    beq @L_801B391C
    bge @L_801B3924
    b @L_801B3964
@L_801B38F4:
    cmpwi r3, 0x4
    bge @L_801B393C
    b @L_801B3934
@L_801B3900:
    cmpwi r3, 0xff
    beq @L_801B395C
    bge @L_801B3964
    cmpwi r3, 0x7
    beq @L_801B3954
    bge @L_801B3964
    b @L_801B394C
@L_801B391C:
    li r3, 0x1
    b @L_801B3978
@L_801B3924:
    li r3, 0x2
    b @L_801B3978
@L_801B392C:
    li r3, 0x3
    b @L_801B3978
@L_801B3934:
    li r3, 0x4
    b @L_801B3978
@L_801B393C:
    li r3, 0x5
    b @L_801B3978
@L_801B3944:
    li r3, 0x6
    b @L_801B3978
@L_801B394C:
    li r3, 0x7
    b @L_801B3978
@L_801B3954:
    li r3, 0x8
    b @L_801B3978
@L_801B395C:
    li r3, 0x0
    b @L_801B3978
@L_801B3964:
    addi r3, r2, -22592
    li r4, 0x453
    addi r5, r2, -22584
    bl fn_80196E10
    li r3, 0x0
@L_801B3978:
    lwz r0, -21704(r13)
    cmpw r3, r0
    ble @L_801B3988
    stw r3, -21704(r13)
@L_801B3988:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
