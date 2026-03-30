    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x8
    stw r0, 0x14(r1)
    bl fn_801DAEF8
    bl fn_801D58E4
    li r3, 0x0
    li r4, 0x64
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801D540C
    li r4, 0x0
    bl fn_800E4014
@L_801D540C:
    lis r4, -32739
    li r3, 0x14
    addi r8, r4, 21604
    li r5, 0x2000
    li r4, 0x58
    li r6, 0x1
    li r7, 0x0
    bl fn_800F07A8
    lis r4, -32698
    addi r4, r4, 29688
    stw r3, 0x66c(r4)
    bl fn_800057A8
    subfic r4, r3, 0x2
    subi r0, r3, 0x2
    or r0, r4, r0
    lis r3, -32698
    rlwinm r0, r0, 1, 31, 31
    stw r0, 29688(r3)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
