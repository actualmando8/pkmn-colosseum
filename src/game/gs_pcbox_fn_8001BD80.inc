    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001BDAC
    lis r3, lbl_803A1D40@ha
    li r0, 0x0
    addi r3, r3, lbl_803A1D40@l
    stb r0, 0x4(r3)
    b @L_8001BDBC
    @L_8001BDAC:
    lis r3, lbl_803A1D40@ha
    li r0, 0x1
    addi r3, r3, lbl_803A1D40@l
    stb r0, 0x4(r3)
    @L_8001BDBC:
    lis r3, lbl_803A1D40@ha
    mr r7, r5
    addi r5, r3, lbl_803A1D40@l
    li r3, 0x2
    stw r6, 0x18(r5)
    clrlwi r6, r4, 24
    li r4, 0x0
    li r5, 0x0
    li r8, 0x1
    bl fn_8001BAC4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
