    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    clrlwi r0, r3, 24
    cmplwi r0, 0x9
    stw r31, 0xc(r1)
    li r31, 0x0
    bgt @L_80135E2C
    lis r3, jumptable_80363AC8@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_80363AC8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r4
    mr r4, r6
    mr r5, r8
    bl fn_8013583C
    b @L_80135E2C
    mr r3, r4
    mr r4, r6
    mr r5, r8
    bl fn_80135024
    b @L_80135E2C
    mr r3, r4
    mr r4, r6
    mr r5, r8
    bl fn_8012A450
    b @L_80135E2C
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    mr r7, r8
    bl fn_80142B24
    b @L_80135E2C
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    mr r7, r8
    bl fn_801254B4
    b @L_80135E2C
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    mr r7, r8
    bl fn_8011BBD8
    b @L_80135E2C
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    mr r7, r8
    bl fn_801F4C14
    mr r31, r3
    b @L_80135E2C
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    mr r7, r8
    bl fn_801F75F8
    b @L_80135E2C
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    mr r7, r8
    bl fn_801FAA58
    @L_80135E2C:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
