    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0xac
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80131428
    mr. r31, r3
    beq @L_8013C5FC
    lis r5, fn_8013C614@ha
    lis r6, fn_8013C718@ha
    lis r7, fn_8013C670@ha
    lis r4, fn_8013CA48@ha
    addi r9, r4, fn_8013CA48@l
    addi r5, r5, fn_8013C614@l
    addi r6, r6, fn_8013C718@l
    addi r7, r7, fn_8013C670@l
    li r4, 0x0
    li r8, 0x0
    li r10, 0x0
    bl fn_80131200
    mr r3, r31
    li r4, 0x0
    bl fn_8013139C
    @L_8013C5FC:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
