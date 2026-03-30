    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    bl fn_8020D8C8
    mr r3, r30
    li r4, 0x0
    bl fn_8020D8B8
    mr r3, r30
    li r4, 0x0
    bl fn_8020D8A8
    li r31, 0x0
    b @L_801F14B0
    @L_801F149C:
    mr r3, r30
    mr r4, r31
    li r5, 0x0
    bl fn_8020D844
    addi r31, r31, 0x1
    @L_801F14B0:
    clrlwi r0, r31, 16
    cmplwi r0, 0x4
    blt @L_801F149C
    mr r3, r30
    li r4, 0x0
    bl fn_8020D888
    mr r3, r30
    li r4, 0x0
    bl fn_8020D878
    mr r3, r30
    li r4, 0x0
    bl fn_8020D898
    mr r3, r30
    li r4, 0x0
    bl fn_8020D868
    mr r3, r30
    li r4, -0x1
    bl fn_8020D78C
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
