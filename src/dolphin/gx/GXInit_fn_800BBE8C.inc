    nofralloc
    mflr r0
    cmpwi r5, 0x2
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    addi r29, r4, 0x0
    stw r28, 0x20(r1)
    addi r28, r3, 0x0
    beq @L_800BBEE0
    bge @L_800BBEC8
    cmpwi r5, 0x1
    bge @L_800BBED4
    b @L_800BBEF4
@L_800BBEC8:
    cmpwi r5, 0x4
    bge @L_800BBEF4
    b @L_800BBEEC
@L_800BBED4:
    li r7, 0x5
    li r30, 0x9
    b @L_800BBEF4
@L_800BBEE0:
    li r7, 0x6
    li r30, 0xa
    b @L_800BBEF4
@L_800BBEEC:
    li r7, 0x7
    li r30, 0xb
@L_800BBEF4:
    li r31, 0x0
    stw r31, 0x8(r1)
    addi r3, r28, 0x0
    addi r4, r29, 0x0
    stw r31, 0xc(r1)
    li r5, 0x0
    li r6, 0x3
    li r8, 0x6
    li r9, 0x6
    li r10, 0x0
    bl fn_800BB780
    stw r31, 0x8(r1)
    addi r4, r29, 0x0
    addi r7, r30, 0x0
    stw r31, 0xc(r1)
    addi r3, r28, 0x1
    li r5, 0x0
    li r6, 0x3
    li r8, 0x6
    li r9, 0x6
    li r10, 0x1
    bl fn_800BB780
    stw r31, 0x8(r1)
    addi r4, r29, 0x0
    addi r3, r28, 0x2
    stw r31, 0xc(r1)
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x1
    bl fn_800BB780
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
