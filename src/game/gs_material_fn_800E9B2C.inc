    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r5, 0xffce
    stw r0, 0x14(r1)
    subi r0, r5, 0x2b63
    stw r31, 0xc(r1)
    mr r31, r4
    addi r4, r31, 0x4
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r6, 0x0(r3)
    and r0, r6, r0
    stw r0, 0x0(r3)
    lwz r5, 0x0(r3)
    lwz r0, 0x0(r31)
    or r0, r5, r0
    stw r0, 0x0(r3)
    bl fn_800E43A4
    mr r3, r30
    addi r4, r31, 0x10
    bl fn_800E4170
    mr r3, r30
    addi r4, r31, 0x1c
    bl fn_800E407C
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E9BB8
    lwz r0, 0x0(r30)
    mr r3, r30
    addi r4, r31, 0x48
    addi r5, r31, 0x54
    xoris r0, r0, 0x2
    addi r6, r31, 0x60
    stw r0, 0x0(r30)
    bl fn_800E732C
    @L_800E9BB8:
    li r0, -0x1
    mr r3, r30
    stw r0, 0x90(r30)
    stw r0, 0xa8(r30)
    lwz r4, 0x28(r31)
    bl fn_800ECCA8
    lfs f1, 0x30(r31)
    mr r3, r30
    bl fn_800ECA78
    lfs f1, 0x34(r31)
    mr r3, r30
    bl fn_800EC9DC
    lwz r4, 0x40(r31)
    mr r3, r30
    bl fn_800ECB74
    lwz r4, 0x2c(r31)
    mr r3, r30
    bl fn_800EC35C
    lfs f1, 0x38(r31)
    mr r3, r30
    bl fn_800EC2A4
    lfs f1, 0x3c(r31)
    mr r3, r30
    bl fn_800EC308
    lwz r4, 0x44(r31)
    mr r3, r30
    bl fn_800EC208
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_800E9C38
    mr r3, r30
    bl fn_800EC990
    @L_800E9C38:
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_800E9C4C
    mr r3, r30
    bl fn_800EC1E4
    @L_800E9C4C:
    mr r3, r30
    bl fn_800ED1CC
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
