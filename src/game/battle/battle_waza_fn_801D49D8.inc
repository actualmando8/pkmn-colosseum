    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmpwi r3, 0x1
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r5
    stw r30, 0x8(r1)
    mr r30, r4
    bne @L_801D4A10
    cmpwi r31, 0xc
    li r3, 0xc
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4A10:
    cmpwi r3, 0x2
    bne @L_801D4A2C
    cmpwi r31, 0x14
    li r3, 0x14
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4A2C:
    cmpwi r3, 0x4
    bne @L_801D4A48
    cmpwi r31, 0x2d
    li r3, 0x2d
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4A48:
    cmpwi r3, 0x8
    bne @L_801D4A58
    mr r3, r31
    b @L_801D4D88
@L_801D4A58:
    cmpwi r3, 0x3
    bne @L_801D4A98
    bl fn_800E0BE4
    lfs f0, 0x0(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4A84
    cmpwi r31, 0xc
    li r3, 0xc
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4A84:
    cmpwi r31, 0x14
    li r3, 0x14
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4A98:
    cmpwi r3, 0x5
    bne @L_801D4AD8
    bl fn_800E0BE4
    lfs f0, 0x0(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4AC4
    cmpwi r31, 0xc
    li r3, 0xc
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4AC4:
    cmpwi r31, 0x2d
    li r3, 0x2d
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4AD8:
    cmpwi r3, 0x9
    bne @L_801D4B0C
    bl fn_800E0BE4
    lfs f0, 0x0(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4B04
    cmpwi r31, 0xc
    li r3, 0xc
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4B04:
    mr r3, r31
    b @L_801D4D88
@L_801D4B0C:
    cmpwi r3, 0x6
    bne @L_801D4B4C
    bl fn_800E0BE4
    lfs f0, 0x4(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4B38
    cmpwi r31, 0x14
    li r3, 0x14
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4B38:
    cmpwi r31, 0x2d
    li r3, 0x2d
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4B4C:
    cmpwi r3, 0xa
    bne @L_801D4B80
    bl fn_800E0BE4
    lfs f0, 0x4(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4B78
    cmpwi r31, 0x14
    li r3, 0x14
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4B78:
    mr r3, r31
    b @L_801D4D88
@L_801D4B80:
    cmpwi r3, 0xc
    bne @L_801D4BB4
    bl fn_800E0BE4
    lfs f0, 0x8(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4BAC
    cmpwi r31, 0x2d
    li r3, 0x2d
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4BAC:
    mr r3, r31
    b @L_801D4D88
@L_801D4BB4:
    cmpwi r3, 0x7
    bne @L_801D4C14
    bl fn_800E0BE4
    lfs f0, 0x0(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4BE0
    cmpwi r31, 0xc
    li r3, 0xc
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4BE0:
    lfs f0, 0x4(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4C00
    cmpwi r31, 0x14
    li r3, 0x14
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4C00:
    cmpwi r31, 0x2d
    li r3, 0x2d
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4C14:
    cmpwi r3, 0xb
    bne @L_801D4C68
    bl fn_800E0BE4
    lfs f0, 0x0(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4C40
    cmpwi r31, 0xc
    li r3, 0xc
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4C40:
    lfs f0, 0x4(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4C60
    cmpwi r31, 0x14
    li r3, 0x14
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4C60:
    mr r3, r31
    b @L_801D4D88
@L_801D4C68:
    cmpwi r3, 0xd
    bne @L_801D4CBC
    bl fn_800E0BE4
    lfs f0, 0x0(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4C94
    cmpwi r31, 0xc
    li r3, 0xc
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4C94:
    lfs f0, 0x8(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4CB4
    cmpwi r31, 0x2d
    li r3, 0x2d
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4CB4:
    mr r3, r31
    b @L_801D4D88
@L_801D4CBC:
    cmpwi r3, 0xe
    bne @L_801D4D10
    bl fn_800E0BE4
    lfs f0, 0x4(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4CE8
    cmpwi r31, 0xc
    li r3, 0xc
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4CE8:
    lfs f0, 0x8(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4D08
    cmpwi r31, 0x14
    li r3, 0x14
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4D08:
    mr r3, r31
    b @L_801D4D88
@L_801D4D10:
    cmpwi r3, 0xf
    bne @L_801D4D84
    bl fn_800E0BE4
    lfs f0, 0x0(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4D3C
    cmpwi r31, 0xc
    li r3, 0xc
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4D3C:
    lfs f0, 0x4(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4D5C
    cmpwi r31, 0xc
    li r3, 0xc
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4D5C:
    lfs f0, 0x8(r30)
    fcmpo cr0, f1, f0
    bge @L_801D4D7C
    cmpwi r31, 0x14
    li r3, 0x14
    bge @L_801D4D88
    mr r3, r31
    b @L_801D4D88
@L_801D4D7C:
    mr r3, r31
    b @L_801D4D88
@L_801D4D84:
    mr r3, r31
@L_801D4D88:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
