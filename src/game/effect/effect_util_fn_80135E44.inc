    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    clrlwi r0, r3, 24
    cmplwi r0, 0x9
    bgt @L_80135F44
    lis r3, jumptable_80363AF0@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_80363AF0@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x0
    b @L_80135F48
    mr r3, r4
    mr r4, r6
    bl fn_80135938
    b @L_80135F48
    mr r3, r4
    mr r4, r6
    mr r5, r7
    bl fn_80135028
    b @L_80135F48
    mr r3, r4
    mr r4, r6
    mr r5, r7
    bl fn_8012A5B0
    b @L_80135F48
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    bl fn_80142CF4
    b @L_80135F48
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    bl fn_8012640C
    b @L_80135F48
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    bl fn_8011BEB4
    b @L_80135F48
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    bl fn_801F54A4
    b @L_80135F48
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    bl fn_801F76B8
    b @L_80135F48
    mr r3, r4
    mr r4, r5
    mr r5, r6
    mr r6, r7
    bl fn_801FB1C0
    b @L_80135F48
    @L_80135F44:
    li r3, 0x0
    @L_80135F48:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
