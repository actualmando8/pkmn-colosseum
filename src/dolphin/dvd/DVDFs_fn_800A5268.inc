    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    addi r30, r4, 0x0
    addi r5, r30, 0x0
    stw r29, 0x14(r1)
    addi r29, r3, 0x0
    addi r4, r29, 0x0
    lwz r31, -24648(r13)
    addi r3, r31, 0x0
    bl fn_800A5108
    cmplw r3, r30
    bne @L_800A52B4
    li r0, 0x0
    add r3, r29, r30
    stb r0, -0x1(r3)
    b @L_800A530C
@L_800A52B4:
    mulli r0, r31, 0xc
    lwz r4, -24660(r13)
    lwzx r0, r4, r0
    rlwinm. r0, r0, 0, 0, 7
    bne @L_800A52D0
    li r0, 0x0
    b @L_800A52D4
@L_800A52D0:
    li r0, 0x1
@L_800A52D4:
    cmpwi r0, 0x0
    beq @L_800A5300
    subi r0, r30, 0x1
    cmplw r3, r0
    bne @L_800A52F4
    li r0, 0x0
    stbx r0, r29, r3
    b @L_800A530C
@L_800A52F4:
    li r0, 0x2f
    stbx r0, r29, r3
    addi r3, r3, 0x1
@L_800A5300:
    li r0, 0x0
    stbx r0, r29, r3
    li r0, 0x1
@L_800A530C:
    mr r3, r0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
