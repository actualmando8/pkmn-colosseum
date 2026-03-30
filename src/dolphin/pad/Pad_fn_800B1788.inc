    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    addi r31, r3, 0x0
    lis r3, lbl_803FC620@ha
    stw r30, 0x18(r1)
    mulli r5, r31, 0x110
    stw r29, 0x14(r1)
    addi r0, r3, lbl_803FC620@l
    mr. r29, r4
    add r30, r0, r5
    blt @L_800B1808
    lwz r3, 0xb8(r30)
    addi r0, r3, 0x200
    stw r0, 0xb8(r30)
    lwz r3, 0xb0(r30)
    addi r0, r3, 0x200
    stw r0, 0xb0(r30)
    lwz r3, 0xb4(r30)
    addi r0, r3, 0x200
    stw r0, 0xb4(r30)
    lwz r3, 0xac(r30)
    subic. r0, r3, 0x1
    stw r0, 0xac(r30)
    ble @L_800B1808
    lis r3, fn_800B1788@ha
    addi r4, r3, fn_800B1788@l
    addi r3, r31, 0x0
    bl fn_800AFD90
    mr. r29, r3
    bge @L_800B1848
    @L_800B1808:
    lwz r0, 0xd0(r30)
    cmplwi r0, 0x0
    bne @L_800B1820
    addi r3, r30, 0x0
    addi r4, r29, 0x0
    bl fn_800B02F4
    @L_800B1820:
    lwz r0, 0xd4(r30)
    cmplwi r0, 0x0
    mr r12, r0
    beq @L_800B1848
    li r0, 0x0
    mtlr r12
    stw r0, 0xd4(r30)
    addi r3, r31, 0x0
    addi r4, r29, 0x0
    blrl
    @L_800B1848:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
