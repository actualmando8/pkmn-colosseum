    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r4
    cmplwi r3, 0x0
    bne @L_8007D7C4
    li r3, 0xa6
    bl fn_80104704
@L_8007D7C4:
    bl fn_801040A0
    lwz r4, 0x0(r3)
    li r0, 0x0
    cmplwi r4, 0x0
    stw r0, 0x4c(r31)
    beq @L_8007D884
    lha r0, 0x6(r31)
    cmpwi r0, 0x1126
    beq @L_8007D7F8
    bge @L_8007D884
    cmpwi r0, 0x795
    beq @L_8007D800
    b @L_8007D884
@L_8007D7F8:
    li r5, 0x0
    b @L_8007D80C
@L_8007D800:
    li r5, 0x1
    b @L_8007D80C
    b @L_8007D884
@L_8007D80C:
    lwz r0, 0xac(r4)
    cmpwi r0, 0x0
    ble @L_8007D82C
    rlwinm r0, r5, 2, 0, 29
    add r3, r4, r0
    lwz r0, 0xa0(r3)
    cmpwi r0, 0x0
    bge @L_8007D834
@L_8007D82C:
    li r3, 0x0
    b @L_8007D840
@L_8007D834:
    lwz r3, 0xb0(r4)
    rlwinm r0, r0, 2, 0, 29
    lwzx r3, r3, r0
@L_8007D840:
    cmplwi r3, 0x0
    beq @L_8007D884
    add r4, r4, r5
    lbz r4, 0xb4(r4)
    bl fn_80082FE4
    mr r30, r3
    lbz r0, 0x71(r30)
    cmplwi r0, 0x0
    beq @L_8007D884
    li r0, 0x3cbe
    li r3, 0x58
    stw r0, 0x4c(r31)
    lbz r4, 0x70(r30)
    bl fn_80132A38
    addi r4, r30, 0x64
    li r3, 0x23
    bl fn_80132A38
@L_8007D884:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
