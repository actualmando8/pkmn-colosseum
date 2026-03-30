    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    clrlwi. r0, r4, 16
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    mr r30, r4
    beq @L_80135194
    cmplwi r0, 0xb
    blt @L_8013519C
    @L_80135194:
    li r3, 0x0
    b @L_80135274
    @L_8013519C:
    cmplwi r3, 0x0
    bne @L_801351D8
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    mr. r31, r3
    bne @L_801351C0
    li r3, 0x0
    b @L_80135274
    @L_801351C0:
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_801351D8
    li r3, 0x0
    b @L_80135274
    @L_801351D8:
    bl fn_80135CD0
    cmplwi r3, 0x0
    bne @L_801351EC
    li r3, 0x0
    b @L_80135274
    @L_801351EC:
    clrlwi r0, r30, 16
    cmplwi r0, 0xa
    bgt @L_80135270
    lis r4, jumptable_80363A9C@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_80363A9C@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    mr r3, r31
    b @L_80135274
    bl fn_80135C78
    b @L_80135274
    bl fn_80135C40
    b @L_80135274
    bl fn_80135C28
    b @L_80135274
    bl fn_80135C10
    fctiwz f0, f1
    stfd f0, 0x8(r1)
    lwz r3, 0xc(r1)
    b @L_80135274
    bl fn_80135BF8
    b @L_80135274
    bl fn_80135BE0
    clrlwi r3, r3, 24
    b @L_80135274
    bl fn_80135BC8
    clrlwi r3, r3, 24
    b @L_80135274
    bl fn_80135BB0
    clrlwi r3, r3, 24
    b @L_80135274
    @L_80135270:
    li r3, 0x0
    @L_80135274:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
