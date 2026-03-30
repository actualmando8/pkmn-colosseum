    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    stw r30, 0x18(r1)
    mr. r30, r3
    lis r3, -32729
    stw r29, 0x14(r1)
    addi r29, r3, 20112
    stw r28, 0x10(r1)
    mr r28, r5
    bne @L_801AA3A0
    addi r3, r29, 0x0
    li r4, 0x1ae
    addi r5, r2, -23048
    bl fn_80196E10
@L_801AA3A0:
    cmplwi r30, 0x0
    beq @L_801AA3E4
    la r4, lbl_8047B2E0(r13)
    b @L_801AA3D4
@L_801AA3B0:
    lwz r0, 0x0(r4)
    cmplw r0, r30
    bne @L_801AA3CC
    lwz r3, 0x0(r4)
    lwz r0, 0x28(r3)
    stw r0, 0x0(r4)
    b @L_801AA3D4
@L_801AA3CC:
    lwz r3, 0x0(r4)
    addi r4, r3, 0x28
@L_801AA3D4:
    lwz r0, 0x0(r4)
    cmplwi r0, 0x0
    bne @L_801AA3B0
    b @L_801AA3EC
@L_801AA3E4:
    li r0, 0x0
    stw r0, lbl_8047B2E0(r13)
@L_801AA3EC:
    mr r3, r30
    li r4, 0x0
    li r5, 0x2c
    bl memset
    li r0, -0x1
    li r3, 0x0
    stw r0, 0x14(r30)
    cmplwi r28, 0x20
    li r0, -0x1
    stw r3, 0x18(r30)
    stw r0, 0x1c(r30)
    ble @L_801AA42C
    addi r3, r29, 0x0
    addi r5, r29, 0xc
    li r4, 0x1b9
    bl fn_80196E10
@L_801AA42C:
    mr r3, r28
    bl fn_801BF138
    cmpwi r3, 0x1
    beq @L_801AA44C
    addi r3, r29, 0x0
    addi r5, r29, 0x18
    li r4, 0x1ba
    bl fn_80196E10
@L_801AA44C:
    stw r28, 0x24(r30)
    lwz r0, 0x24(r30)
    lwz r3, 0x24(r30)
    add r4, r0, r31
    subi r4, r4, 0x1
    subi r0, r3, 0x1
    andc r0, r4, r0
    stw r0, 0x20(r30)
    lwz r0, lbl_8047B2E0(r13)
    stw r0, 0x28(r30)
    stw r30, lbl_8047B2E0(r13)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
