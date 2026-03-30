    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    li r31, 0x0
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    lwz r4, lbl_8047B610(r13)
    lbz r0, 0x3(r4)
    lbz r30, 0x2(r4)
    cmplwi r0, 0x7
    bgt @L_80223B7C
    lis r4, jumptable_8039A200@ha
    rlwinm r0, r0, 2, 0, 29
    addi r4, r4, jumptable_8039A200@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r5, 0xe6
    b @L_80223B80
    li r5, 0xe7
    b @L_80223B80
    li r5, 0xea
    b @L_80223B80
    li r5, 0xe8
    b @L_80223B80
    li r5, 0xe9
    b @L_80223B80
    li r5, 0xeb
    b @L_80223B80
    li r5, 0xec
    b @L_80223B80
@L_80223B7C:
    li r5, 0x0
@L_80223B80:
    li r4, 0x0
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r30, 0, 24, 31
    lwz r4, lbl_8047B610(r13)
    cmpwi r0, 0x3
    rlwinm r5, r3, 0, 24, 31
    lbz r3, 0x4(r4)
    beq @L_80223C0C
    bge @L_80223BC0
    cmpwi r0, 0x1
    beq @L_80223BE4
    bge @L_80223BF8
    cmpwi r0, 0x0
    bge @L_80223BD0
    b @L_80223C44
@L_80223BC0:
    cmpwi r0, 0x5
    beq @L_80223C34
    bge @L_80223C44
    b @L_80223C20
@L_80223BD0:
    rlwinm r0, r3, 0, 24, 31
    cmplw r5, r0
    bne @L_80223C44
    li r31, 0x1
    b @L_80223C44
@L_80223BE4:
    rlwinm r0, r3, 0, 24, 31
    cmplw r5, r0
    beq @L_80223C44
    li r31, 0x1
    b @L_80223C44
@L_80223BF8:
    rlwinm r0, r3, 0, 24, 31
    cmplw r5, r0
    ble @L_80223C44
    li r31, 0x1
    b @L_80223C44
@L_80223C0C:
    rlwinm r0, r3, 0, 24, 31
    cmplw r5, r0
    bge @L_80223C44
    li r31, 0x1
    b @L_80223C44
@L_80223C20:
    rlwinm r0, r3, 0, 24, 31
    and. r0, r5, r0
    beq @L_80223C44
    li r31, 0x1
    b @L_80223C44
@L_80223C34:
    rlwinm r0, r3, 0, 24, 31
    and. r0, r5, r0
    bne @L_80223C44
    li r31, 0x1
@L_80223C44:
    rlwinm. r0, r31, 0, 24, 31
    beq @L_80223C54
    lwz r0, 0x5(r4)
    b @L_80223C5C
@L_80223C54:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x9
@L_80223C5C:
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
