    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lis r3, lbl_80267680@ha
    lha r4, 0x6(r31)
    addi r3, r3, lbl_80267680@l
    lwz r0, 0x0(r3)
    cmpw r4, r0
    bne @L_80055FBC
    li r0, 0x0
    b @L_80055FF0
    @L_80055FBC:
    addi r3, r3, 0x4
    lwz r0, 0x0(r3)
    cmpw r4, r0
    bne @L_80055FD4
    li r0, 0x1
    b @L_80055FF0
    @L_80055FD4:
    addi r3, r3, 0x4
    lwz r0, 0x0(r3)
    cmpw r4, r0
    bne @L_80055FEC
    li r0, 0x2
    b @L_80055FF0
    @L_80055FEC:
    li r0, -0x1
    @L_80055FF0:
    cmpwi r0, 0x0
    bge @L_80056000
    li r3, 0x0
    b @L_80056070
    @L_80056000:
    extsb r4, r0
    li r3, 0x0
    bl fn_80134A98
    cmplwi r3, 0x0
    bne @L_8005601C
    li r3, 0x0
    b @L_80056070
    @L_8005601C:
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    li r3, 0xce
    bl fn_800FA444
    srwi r0, r3, 16
    lha r3, 0x54(r31)
    extsh r6, r0
    li r4, 0x0
    srwi r5, r6, 31
    srwi r0, r3, 31
    add r6, r5, r6
    srawi r7, r6, 1
    add r0, r0, r3
    srawi r0, r0, 1
    li r5, -0x1
    subf r0, r7, r0
    li r6, 0xce
    extsh r3, r0
    bl fn_800FB680
    li r3, 0x0
    @L_80056070:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
