    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_8006B420
    rlwinm r0, r30, 0, 16, 31
    mr r31, r3
    cmpwi r0, 0xaf
    beq @L_80077CAC
    bge @L_80077CB4
    cmpwi r0, 0x0
    beq @L_80077CA4
    b @L_80077CB4
@L_80077CA4:
    li r3, 0x1
    b @L_80077CBC
@L_80077CAC:
    li r3, 0x0
    b @L_80077CBC
@L_80077CB4:
    mr r3, r30
    bl fn_80142984
@L_80077CBC:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80077CD0
    li r3, 0x0
    b @L_80077D70
@L_80077CD0:
    lwz r0, 0x8(r31)
    cmpwi r0, 0x1
    beq @L_80077D00
    bge @L_80077CEC
    cmpwi r0, 0x0
    bge @L_80077CF8
    b @L_80077D6C
@L_80077CEC:
    cmpwi r0, 0x3
    bge @L_80077D6C
    b @L_80077D14
@L_80077CF8:
    li r3, 0x1
    b @L_80077D70
@L_80077D00:
    rlwinm r0, r30, 0, 16, 31
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r3, r0, 0, 24, 31
    b @L_80077D70
@L_80077D14:
    lis r3, lbl_802EE458@ha
    lwz r0, lbl_80478928(r13)
    addi r4, r3, lbl_802EE458@l
    li r5, 0x0
    rlwinm r3, r30, 0, 16, 31
    mtctr r0
    cmplwi r0, 0x0
    ble @L_80077D64
@L_80077D34:
    lhz r0, 0x0(r4)
    cmplw r3, r0
    bne @L_80077D58
    add r3, r31, r5
    lbz r0, 0x18(r3)
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r3, r0, 0, 24, 31
    b @L_80077D70
@L_80077D58:
    addi r4, r4, 0x2
    addi r5, r5, 0x1
    bdnz @L_80077D34
@L_80077D64:
    li r3, 0x1
    b @L_80077D70
@L_80077D6C:
    li r3, 0x0
@L_80077D70:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
