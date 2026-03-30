    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    lis r3, -32698
    addi r27, r3, 28752
    lwz r3, 0x28(r27)
    cmplwi r3, 0x0
    beq @L_801C7124
    bl fn_801C75EC
@L_801C7124:
    lis r3, -32698
    addi r28, r3, 28752
    lwz r3, 0x2c(r28)
    cmplwi r3, 0x0
    beq @L_801C713C
    bl fn_801C75EC
@L_801C713C:
    lis r3, -32698
    addi r29, r3, 28752
    lwz r3, 0x30(r29)
    cmplwi r3, 0x0
    beq @L_801C7154
    bl fn_801C75EC
@L_801C7154:
    lis r3, -32698
    addi r30, r3, 28752
    lwz r3, 0x34(r30)
    cmplwi r3, 0x0
    beq @L_801C716C
    bl fn_801C75EC
@L_801C716C:
    lis r3, -32698
    addi r31, r3, 28752
    lwz r3, 0x38(r31)
    cmplwi r3, 0x0
    beq @L_801C7184
    bl fn_801C75EC
@L_801C7184:
    li r0, 0x0
    stw r0, 0x28(r27)
    stw r0, 0x2c(r28)
    stw r0, 0x30(r29)
    stw r0, 0x34(r30)
    stw r0, 0x38(r31)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
