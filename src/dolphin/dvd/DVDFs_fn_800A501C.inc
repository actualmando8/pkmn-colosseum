    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x98(r1)
    stw r31, 0x94(r1)
    addi r31, r4, 0x0
    stw r30, 0x90(r1)
    addi r30, r3, 0x0
    bl fn_800A4D28
    cmpwi r3, 0x0
    bge @L_800A5070
    addi r3, r1, 0x10
    li r4, 0x80
    bl fn_800A5268
    lis r3, -32719
    crxor 6, 6, 6
    addi r3, r3, 6584
    addi r4, r30, 0x0
    addi r5, r1, 0x10
    bl OSReport
    li r3, 0x0
    b @L_800A50CC
@L_800A5070:
    mulli r5, r3, 0xc
    lwz r3, -24660(r13)
    lwzx r0, r3, r5
    rlwinm. r0, r0, 0, 0, 7
    bne @L_800A508C
    li r0, 0x0
    b @L_800A5090
@L_800A508C:
    li r0, 0x1
@L_800A5090:
    cmpwi r0, 0x0
    beq @L_800A50A0
    li r3, 0x0
    b @L_800A50CC
@L_800A50A0:
    add r3, r3, r5
    lwz r4, 0x4(r3)
    li r0, 0x0
    li r3, 0x1
    stw r4, 0x30(r31)
    lwz r4, -24660(r13)
    add r4, r4, r5
    lwz r4, 0x8(r4)
    stw r4, 0x34(r31)
    stw r0, 0x38(r31)
    stw r0, 0xc(r31)
@L_800A50CC:
    lwz r0, 0x9c(r1)
    lwz r31, 0x94(r1)
    lwz r30, 0x90(r1)
    addi r1, r1, 0x98
    mtlr r0
    blr
