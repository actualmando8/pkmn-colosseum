    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r4, 0x0
    lis r5, -32697
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    addi r31, r5, -15984
    beq @L_801E54CC
    lis r5, -32738
    addi r6, r31, 0x58
    addi r0, r5, 20324
    mr r8, r3
    mr r5, r4
    addi r3, r31, 0x1058
    mr r4, r0
    li r7, 0x1000
    li r9, 0x1
    addi r6, r6, 0x1000
    bl fn_800A19CC
    cmpwi r3, 0x0
    bne @L_801E5504
    li r3, 0x0
    b @L_801E5534
@L_801E54CC:
    lis r4, -32738
    addi r6, r31, 0x58
    mr r8, r3
    addi r3, r31, 0x1058
    addi r4, r4, 20820
    li r5, 0x0
    li r7, 0x1000
    li r9, 0x1
    addi r6, r6, 0x1000
    bl fn_800A19CC
    cmpwi r3, 0x0
    bne @L_801E5504
    li r3, 0x0
    b @L_801E5534
@L_801E5504:
    addi r3, r31, 0x38
    addi r4, r31, 0xc
    li r5, 0x3
    bl fn_8009F1D0
    addi r3, r31, 0x18
    addi r4, r31, 0x0
    li r5, 0x3
    bl fn_8009F1D0
    li r0, 0x1
    li r3, 0x1
    stw r0, -21400(r13)
    stw r0, -21396(r13)
@L_801E5534:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
