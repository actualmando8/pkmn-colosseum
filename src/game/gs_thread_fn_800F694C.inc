    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    lwz r3, 0x14(r3)
    lwz r31, 0x0(r3)
    addi r0, r3, 0x4
    stw r0, 0x14(r28)
    lwz r3, 0x14(r28)
    lhz r30, 0x0(r3)
    addi r0, r3, 0x2
    stw r0, 0x14(r28)
    lwz r3, 0x14(r28)
    lhz r29, 0x0(r3)
    addi r0, r3, 0x2
    stw r0, 0x14(r28)
    lwz r4, 0x28(r28)
    lwz r5, 0x14(r28)
    cmpwi r4, 0x40
    ble @L_800F69C0
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F69D4
    @L_800F69C0:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r28)
    add r3, r28, r0
    stw r5, 0x6c(r3)
    @L_800F69D4:
    lwz r4, 0x28(r28)
    lwz r5, 0x1c(r28)
    cmpwi r4, 0x40
    ble @L_800F69F8
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6A0C
    @L_800F69F8:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r28)
    add r3, r28, r0
    stw r5, 0x6c(r3)
    @L_800F6A0C:
    lwz r4, 0x28(r28)
    cmpwi r4, 0x40
    ble @L_800F6A2C
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6A40
    @L_800F6A2C:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r28)
    add r3, r28, r0
    stw r29, 0x6c(r3)
    @L_800F6A40:
    addi r3, r29, 0x3
    lwz r0, 0x28(r28)
    add r3, r30, r3
    subf r0, r3, r0
    stw r0, 0x1c(r28)
    lwz r4, 0x0(r28)
    lhz r0, 0x4(r4)
    cmplw r31, r0
    blt @L_800F6A7C
    lis r3, lbl_8027115C@ha
    mr r4, r31
    addi r3, r3, lbl_8027115C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6A90
    @L_800F6A7C:
    slwi r0, r31, 2
    add r3, r4, r0
    lwz r0, 0x18(r3)
    add r0, r4, r0
    stw r0, 0x14(r28)
    @L_800F6A90:
    lwz r0, 0x24(r1)
    li r3, 0x1
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
